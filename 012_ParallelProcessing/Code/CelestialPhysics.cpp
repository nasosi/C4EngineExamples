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

// kJobNonpersistent tells the engine to automatically destroy the job after execution. Since we create new jobs every frame, manual
// lifetime management is unnecessary.
SimulationJob::SimulationJob( ExecuteCallback* execCallback, void* cookie, Range<int32> bodyIndexRange ) :
    BatchJob( execCallback, cookie, kJobNonpersistent ),
    bodyIndexRange( bodyIndexRange )
{
}


CelestialPhysicsController::CelestialPhysicsController() : Controller( kControllerCelestialPhysics )
{
}


// Core simulation kernel. Multiple instances of this function execute in parallel each frame.
void CelestialPhysicsController::SimulateGalaxy( Job* job, void* cookie )
{
    // Jobs execute static functions, so they cannot access 'this' directly. We pass the controller instance through the generic "cookie"
    // pointer and cast it back to CelestialPhysicsController here.
    CelestialPhysicsController* controller = static_cast<CelestialPhysicsController*>( cookie );

    if ( !controller )
    {
        return;
    }

    // This simulation uses Modified Newtonian Dynamics (MOND), an alternative to dark matter models at galactic scales.
    // The simulation uses arbitrary units rather than real world units.
    SimulationJob* simulationJob = static_cast<SimulationJob*>( job );

    // Range of bodies processed by this worker thread.
    const Range<int32>& range = simulationJob->GetBodyIndexRange();

    const int32 bodyCount = controller->bodyArray.GetArrayElementCount();

    for ( int32 a = range.min; a < range.max; a++ )
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
    // The C4 Engine job system is designed for short-lived per-frame work. Long-running independent systems ( for example network services,
    // file watchers, pathfinding workers, etc.) should use dedicated C4::Thread instances.
    //
    // Every body interacts gravitationally with every other body, giving the simulation an O(N²) computational cost.
    //
    // To parallelize the workload, the bodies are divided into chunks and processed independently across worker threads.
    //
    // Example:
    //   Job 0 -> bodies [0 ... 249]
    //   Job 1 -> bodies [250 ... 499]
    //   Job 2 -> bodies [500 ... 749]

    // Clamp to at least one worker so the simulation still runs when multithreading is unavailable.
    const int32 workerCount = Max( TheThreadMgr->GetWorkerThreadCount(), 1 );

    // We never want more jobs than bodies. For example:4 bodies + 16 workers would create many empty jobs, which only adds scheduling
    // overhead.
    const int32 jobCount = Min( workerCount, bodyCount );

    // Compute the number of bodies per job using integer ceil-division. This guarantees all bodies are assigned even when the division is
    // uneven.
    const int32 chunkSize = ( bodyCount + jobCount - 1 ) / jobCount;

    // Create and submit jobs. Each job receives a half-open range: [begin, end), where 'begin' is inclusive and 'end' is exclusive.
    // Example:
    // Range(0, 100) processes bodies 0 through 99.
    for ( int32 begin = 0; begin < bodyCount; begin += chunkSize )
    {
        // Clamp the end index so we never exceed the array bounds.
        const int32 end = Min( begin + chunkSize, bodyCount );

        // Create a simulation job responsible for this body range. The computation function is static, so we pass 'this' through the cookie
        // pointer for context access.
        SimulationJob* job = new SimulationJob( &SimulateGalaxy, this, Range<int32>( begin, end ) );

        // Submit the job to the engine's worker thread system. The engine schedules these jobs in parallel and associates them with the
        // simulation batch.
        TheThreadMgr->SubmitJob( job, &simulationBatch );
    }

    // Block until all simulation jobs in this batch complete. Acceleration data must be fully computed before integration begins.
    TheThreadMgr->FinishBatch( &simulationBatch );

    // Integrate using Symplectic Euler.
    const float dt = float( TheTimeMgr->GetDeltaTime() );

    for ( auto& body : bodyArray )
    {
        // Symplectic Euler: First update velocity from acceleration...
        body.velocity += body.acceleration * dt;

        // ...then update position using the new velocity.
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