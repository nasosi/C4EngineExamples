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


#include "World.hpp"
#include "CelestialPhysics.hpp"


const Point3D   cameraDefaultPosition    = Point3D( 0.0f, 0.0f, 1.7f );
constexpr float cameraFoV_deg            = 65.0f;
constexpr float cameraAspectRatio        = 1.0f;
constexpr float spectatorCollisionRadius = 0.3f;


float CalculateCameraFocalLength( float fovAngle )
{
    float angle = Fmin( Fmax( fovAngle, 45.0f ), 90.0f );
    return ( 1.0F / Tan( angle * ( Const::radians * 0.5F ) ) );
}


void GameWorld::SetupSpectatorCamera()
{
    SetWorldCamera( &spectatorCamera );

    float   azimuth  = 0;
    float   altitude = 0;
    Point3D position = cameraDefaultPosition;

    Marker* marker = GetRootNode()->GetFirstMarker();
    while ( marker )
    {
        if ( ( marker->GetMarkerType() == kMarkerLocator ) && ( marker->NodeEnabled() ) )
        {
            const auto locator = static_cast<const LocatorMarker*>( marker );
            if ( locator->GetLocatorType() == 'spec' )
            {
                const Vector3D direction = marker->GetWorldTransform()[ 0 ];

                azimuth  = Arctan( direction.y, direction.x );
                altitude = Arctan( direction.z, Sqrt( direction.x * direction.x + direction.y * direction.y ) );

                position = marker->GetWorldPosition();
                break;
            }
        }

        marker = marker->GetNextListElement();
    }

    spectatorCamera.SetCameraAzimuth( azimuth );
    spectatorCamera.SetCameraAltitude( altitude );
    spectatorCamera.SetNodePosition( position );
}


GameWorld::GameWorld( const char* name ) :
    World( name ),
    spectatorCamera( CalculateCameraFocalLength( cameraFoV_deg ), cameraAspectRatio, spectatorCollisionRadius )
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::SetupCelestialSimulation( int numBodies )
{
    using namespace Random;

    struct StarProperties
    {
        Vector3D position = Vector3D::zero;
        float    mass     = 0.0f;
    };


    auto ComputeOrbitalVelocity = []( const Vector3D& position ) -> Vector3D
    {
        const float planarDistanceSq = position.x * position.x + position.y * position.y;

        if ( planarDistanceSq <= initialVelocityDistCutoff * initialVelocityDistCutoff )
        {
            return Vector3D::zero;
        }

        const float planarDistance = Sqrt( planarDistanceSq );

        Vector3D tangent( -position.y, position.x, 0.0f );
        tangent.Normalize();

        const float orbitalSpeed = maxOrbitalSpeed * planarDistance / Sqrt( planarDistanceSq + missingMassScaleRadiusSq );

        return tangent * orbitalSpeed;
    };


    auto GenerateStarProperties = [ & ]() -> StarProperties
    {
        const float bulgeProbability = galacticBulgeRadius / galacticDiskRadius;

        const float r = RandomFloat( 0.0f, 1.0f );

        // Bulge stars are biased toward larger masses and spherical distribution.
        if ( r < bulgeProbability )
        {
            const float distanceFromCenter = galacticBulgeRadius * Pow( RandomFloat( 0.0f, 1.0f ), 2.0f );

            StarProperties properties;
            properties.position = distanceFromCenter * RandomUnitVector3D();
            properties.mass     = RandomFloat( galacticBulgeMinMass, galacticBulgeMaxMass );

            return properties;
        }

        // Disk stars follow an exponential radial distribution.Clamp the random sample to avoid log( 0 ) and extreme outliers.
        const float u                  = Clamp( RandomFloat( 0.0f, 1.0f ), randomEpsilon, 1.0f - randomEpsilon );
        const float distanceFromCenter = -galacticDiskRadius * Log( 1.0f - u );
        const float azimuth            = RandomFloat( 0.0f, Math::two_pi );

        StarProperties properties;
        properties.position = Vector3D( Cos( azimuth ) * distanceFromCenter,
                                        Sin( azimuth ) * distanceFromCenter,
                                        RandomFloat( -galacticDiskThickness, galacticDiskThickness ) );
        properties.mass     = galacticDiskMassScale * Pow( RandomFloat( 0.0f, 1.0f ), 3.0f ) + galacticDiskMinMass;

        return properties;
    };

    // The controller is responsible for submitting and managing celestial physics simulation jobs each frame. Ownership is transferred to
    // the physics node.
    CelestialPhysicsController* controller = new CelestialPhysicsController();

    // Root node for the celestial physics simulation system. Ownership is transferred to the world through AddNewNode().
    auto* celestialPhysicsNode = new Node();
    celestialPhysicsNode->SetController( controller );

    auto AddStar = [ & ]( float density, float mass, const Vector3D& position, const Vector3D& velocity )
    {
        const float radius = Pow( 3.0f * mass / ( Math::four_pi * density ), 1.0f / 3.0f );

        auto* star = new SphereGeometry( Vector3D( radius, radius, radius ) );
        star->SetNodePosition( Point3D( position ) );

        SphereGeometryObject* object = star->GetObject();
        object->SetCollisionExclusionMask( kCollisionExcludeAll );
        object->BuildPrimitive( star );
        object->SetGeometryFlags( object->GetGeometryFlags() | kGeometryDynamic );
        controller->AddBody( star, mass, velocity );


        AddNewNode( star );
    };

    // The galaxy core mass
    AddStar( defaultStarSystemDensity, galacticCoreMass, Point3D::zero, Vector3D::zero );

    while ( numBodies-- > 0 )
    {
        const StarProperties properties = GenerateStarProperties();

        AddStar( defaultStarSystemDensity, properties.mass, properties.position, ComputeOrbitalVelocity( properties.position ) );
    }

    AddNewNode( celestialPhysicsNode );
}


WorldResult GameWorld::PreprocessWorld()
{
    WorldResult result = World::PreprocessWorld();
    if ( result != kWorldOkay )
    {
        return ( result );
    }

    SetupSpectatorCamera();

    SetupCelestialSimulation( numStars );

    return kWorldOkay;
}
