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


#pragma once


#include <C4Controller.h>
#include <C4Threads.h>

constexpr float defaultStarSystemDensity  = 100.0f; // Mass density of a star system
constexpr float galacticCoreMass          = 200.0f;
constexpr float galacticDiskRadius        = 150.0f;
constexpr float galacticBulgeRadius       = 15.0f;
constexpr float initalVelocityDistCuttoff = 1.0f; // Systems closer than 1.0f, will have an initial velocity of 0.0f.
constexpr float maxOrbitalSpeed          = 0.05f;
constexpr float missingMassScaleRadiusSq = 10000.0f; // Controls initial conditions behavior for stars away from the galactic center
constexpr float gravitationalConstant    = 1e-8f; // G
constexpr float softeningLengthSq        = 0.2f; // A softening factor to avoid singularities when stars are very close to each other.
constexpr float a0                       = 1e-6f; // MOdified Newton Dynamics (MOND) scale.
constexpr int   numStars                 = 6000;


namespace C4
{
    class SphereGeometry; // We forward declare SphereGeometry so that we don't include the header
}


using namespace C4;

enum : ControllerType
{
    kControllerCelestialPhysics = 'ccps'
};


class SimulationJob final : public BatchJob
{
private:


public:

    SimulationJob( ExecuteCallback* execCallback, void* cookie, Range<int32> bodyIndexRange );
    ~SimulationJob() = default;

    Range<int32> bodyIndexRange;
};


class CelestialPhysicsController : public Controller
{
private:

    // We could use a Strcture of Arrays approach (SoAs), but we won't see any benefit unless we also implement other changes
    // For the current example we will keep things simple.
    struct CelestialBody
    {
        SphereGeometry* geometry;
        float           mass;
        Vector3D        velocity;

        alignas( 64 ) Point3D position;
        alignas( 64 ) Vector3D acceleration;
    };

    Array<CelestialBody> bodyArray;

    Batch       simulationBatch; // The batch to associate our jobs with each other
    static void SimulateGalaxy( Job* job, void* cookie );

public:

    CelestialPhysicsController();

    // Called on every frame. We will use this to submit the per frame jobs.
    void MoveController() override;

    void AddBody( SphereGeometry* geom, float mass, const Vector3D& velocity );
};
