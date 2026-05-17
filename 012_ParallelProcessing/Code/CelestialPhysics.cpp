// MIT License
//
// Copyright (c) 2023-2026 Athanasios Iliopoulos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// This software may contain work that is licensed under different terms. In that
// event, appropriate license files will be included in close folder proximity
// to said work. You need to ensure that your use of this software is compatible
// with the license of any other work that may be part of this software.


#include "CelestialPhysics.hpp"

#include <C4World.h>

SimulationJob::SimulationJob( ExecuteCallback* execCallback, void* cookie, Range<int32> bodyIndexRange ) :
    BatchJob( execCallback, cookie, kJobNonpersistent ),
    bodyIndexRange( bodyIndexRange )
{
}


CelestialPhysicsController::CelestialPhysicsController() : Controller( kControllerCelestialPhysics )
{
}


// This is the core computational function (kernel). We will be executing multiple of these in parallel per frame
void CelestialPhysicsController::SimulateGalaxy( Job* job, void* cookie )
{
    // This function is static and does not have access to 'this'.
    //
    // To work around that, we pass a generic pointer ("cookie") when the job is
    // created. This pointer stores the CelestialPhysicsController instance.
    //
    // Here we cast it back so we can access the controller's data (bodyArray,
    // simulation parameters, etc.).
    //
    // This pattern allows the job system to remain generic: the cookie can point
    // to any user-defined context, but in this case it is always a
    // CelestialPhysicsController instance.

    CelestialPhysicsController* controller = static_cast<CelestialPhysicsController*>( cookie );

    if ( !controller )
    {
        return;
    }


    // This simulation is using Modified Newton Dynamics (https://en.wikipedia.org/wiki/Modified_Newtonian_dynamics)
    // to account for the apparent missing matter in galactic scale dynamics. This is an alternative to Dark Matter theories.
    // We are not using physical units anywhere in these simulations.
    SimulationJob* simulationJob = static_cast<SimulationJob*>( job );

    const Range<int32>& range     = simulationJob->bodyIndexRange;
    const int32         bodyCount = controller->bodyArray.GetArrayElementCount();

    for ( int32 a = range.min; a < range.max; ++a )
    {
        CelestialBody& bodyA = controller->bodyArray[ a ];
        if ( bodyA.mass <= 0.0F )
        {
            continue;
        }

        const Point3D& pA = bodyA.position;
        Vector3D       acceleration( 0.0F, 0.0F, 0.0F );

        for ( int32 b = 0; b < bodyCount; ++b )
        {
            if ( a == b )
            {
                continue;
            }

            const CelestialBody& bodyB = controller->bodyArray[ b ];
            if ( bodyB.mass <= 0.0F )
            {
                continue;
            }

            const Point3D& pB                    = bodyB.position;
            const Vector3D delta                 = pB - pA;
            const float    distanceSq            = Dot( delta, delta ) + softeningLengthSq;
            const float    distance              = Sqrt( distanceSq );
            const Vector3D direction             = delta / distance;
            const float    newtonianAcceleration = ( gravitationalConstant * bodyB.mass ) / distanceSq;
            const float    mondRatio             = Fmax( float( double( newtonianAcceleration ) / double( a0 ) ), 1.0e-8f );
            const float    effectiveAcceleration = newtonianAcceleration * 0.5f * ( 1.0f + Sqrt( 1.0f + 4.0f / mondRatio ) );

            acceleration += direction * effectiveAcceleration;
        }

        bodyA.acceleration = acceleration;
    }
}


void CelestialPhysicsController::MoveController()
{
    const int32 bodyCount = bodyArray.GetArrayElementCount();

    if ( bodyCount < 2 )
    {
        return;
    }

    // IMPORTANT:
    // This entire simulation runs once per frame. The C4 Engine job system is designed around a per-frame batch execution model.
    // For long-running or independent tasks (e.g. network services or file system watchers), use a dedicated C4::Thread instead of the job
    // system.
    // The galaxy simulation is the most expensive part of the update because every body gravitationally interacts with every other body
    // (O(N²) cost).
    // All code below runs each frame before the integration step (velocity/position updates), ensuring that each frame uses freshly
    // computed accelerations.
    // To improve performance, the workload is split across multiple worker threads. Each thread processes a subset ("chunk") of the bodies
    // independently.
    //
    // Example:
    //
    //   Job 0 -> bodies [0 ... 249]
    //   Job 1 -> bodies [250 ... 499]
    //   Job 2 -> bodies [500 ... 749]
    //
    // Each job computes accelerations independently in parallel.


    // Get the number of worker threads available in the engine. We clamp to at least 1 so the simulation still works even if threading is
    // disabled or unavailable.
    const int32 workerCount = Max( TheThreadMgr->GetWorkerThreadCount(), 1 );


    // We never want more jobs than bodies. For example: 4 bodies + 16 workers, would create many empty jobs, which wastes scheduling
    // overhead. So we cap the job count to the number of bodies.
    const int32 jobCount = Min( workerCount, bodyCount );


    // Compute how many bodies each job should process.This uses integer ceil-division so all bodies are covered even when
    // the division is uneven. Example: 1000 bodies / 6 jobs becomes: chunkSize = 167, so the final job simply processes fewer bodies.
    const int32 chunkSize = ( bodyCount + jobCount - 1 ) / jobCount;


    // Create and submit jobs.
    //
    // Each job receives a range: [begin, end) where 'begin' is inclusive, 'end' is exclusive. Example:
    //   Range(0, 100)
    // processes bodies:
    //   0 -> 99
    //
    for ( int32 begin = 0; begin < bodyCount; begin += chunkSize )
    {
        // Clamp the end so we never go past the array size.
        const int32 end = Min( begin + chunkSize, bodyCount );

        // Create a simulation job responsible for this body range. Notice that the function that is responsible for
        // the computatuion is a static member. We are passing a "cookie" ('this' here), that the function will
        // use to retrieve additional data.
        SimulationJob* job = new SimulationJob( &SimulateGalaxy, this, Range<int32>( begin, end ) );

        // Submit the job to the engine's worker thread system. The engine schedules these jobs to run in parallel, and assosicates them
        // with the simulationBatch.
        TheThreadMgr->SubmitJob( job, &simulationBatch );
    }

    // Wait for all jobs in this batch to complete. This is a blocking synchronization point. It ensures that all acceleration computations
    // are finished before we proceed to the integration step (velocity and position updates).
    TheThreadMgr->FinishBatch( &simulationBatch );

    // Finally we integrate using symplectic Euler
    const float dt = float( TheTimeMgr->GetDeltaTime() );
    for ( auto& body : bodyArray )
    {
        body.velocity += body.acceleration * dt;
        body.position += body.velocity * dt;

        body.geometry->SetNodePosition( body.position );
        body.geometry->InvalidateNode();
    }
}


void CelestialPhysicsController::AddBody( SphereGeometry* geom, float mass, const Vector3D& velocity )
{
    CelestialBody celestialBody {};
    celestialBody.geometry = geom;
    celestialBody.mass     = mass;
    celestialBody.velocity = velocity;
    celestialBody.position = geom->GetNodeTransform().GetTranslation();

    bodyArray.AppendArrayElement( celestialBody );
}