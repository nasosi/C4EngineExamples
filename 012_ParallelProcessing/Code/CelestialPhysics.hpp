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


// Approximate mass density assigned to generated star systems.
constexpr float defaultStarSystemDensity = 100.0f;

// Mass assigned to the galactic core region.
constexpr float galacticCoreMass = 200.0f;

// Approximate radius of the visible galactic disk.
constexpr float galacticDiskRadius = 150.0f;

// Radius of the dense central bulge.
constexpr float galacticBulgeRadius = 15.0f;

// Bodies closer than this distance to the center begin with zero initial velocity.
// This prevents unstable orbital speeds very close to the core.
constexpr float initialVelocityDistCutoff = 1.0f;

// Maximum initial orbital speed assigned to generated bodies.
constexpr float maxOrbitalSpeed = 0.05f;

// Controls how orbital velocity falls off with distance from the galactic center.
// Larger values produce flatter galactic rotation curves.
constexpr float missingMassScaleRadiusSq = 10000.0f;

// Gravitational constant used by the simulation.
// The simulation uses arbitrary units rather than real-world units.
constexpr float gravitationalConstant = 1e-8f;

// Small softening factor added to distance calculations to avoid
// singularities and extremely large forces at short distances.
constexpr float softeningLengthSq = 0.2f;

// MOND acceleration scale parameter.
constexpr float a0 = 1e-6f;

// Number of simulated star systems.
constexpr int numStars = 6000;


namespace C4
{
    // Forward declaration avoids including the full geometry header here, reducing compile-time dependencies.
    class SphereGeometry;
} // namespace C4


using namespace C4;

enum : ControllerType
{
    kControllerCelestialPhysics = 'ccps'
};


// Controller responsible for updating the galaxy simulation each frame.
class CelestialPhysicsController : public Controller
{
private:

    // Represents a single simulated celestial body. A Structure of Arrays (SoA) layout could improve cache efficiency, but for this
    // example we keep the data layout simple and readable.
    struct alignas( 64 ) CelestialBody
    {
        SphereGeometry* geometry;

        float    mass;
        Vector3D velocity;

        Point3D  position;
        Vector3D acceleration;
    };

    // All simulated bodies in the galaxy.
    Array<CelestialBody> bodyArray;

    // Batch used to group all simulation jobs submitted during a frame.
    Batch simulationBatch;

    // Main parallel simulation kernel executed by each worker thread.
    static void SimulateGalaxy( Job* job, void* cookie );

public:

    CelestialPhysicsController();

    // Called once per frame to advance the simulation.
    void MoveController() override;

    // Adds a new body to the simulation.
    void AddBody( SphereGeometry* geom, float mass, const Vector3D& velocity );
};


// Job object representing work assigned to a worker thread.
class SimulationJob final : public BatchJob
{
private:

    // Half-open range [min, max) of bodies processed by this job.
    Range<int32> bodyIndexRange;

public:

    // execCallback: Function executed by the worker thread. bodyIndexRange: Portion of the body array assigned to this job.
    SimulationJob( ExecuteCallback* execCallback, void* cookie, Range<int32> bodyIndexRange );

    ~SimulationJob() = default;

    const Range<int32>& GetBodyIndexRange() const
    {
        return bodyIndexRange;
    }
};