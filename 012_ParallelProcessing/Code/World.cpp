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
#include "Game.hpp"

#include "C4Computation.h"

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

void GameWorld::SetupCelestialSimulation( int num )
{
    using namespace Random;

    // The Controller will be the main physics simulator engine and responsible submitting jobs for parallel processing every frame.
    CelestialPhysicsController* controller = new CelestialPhysicsController();

    // This Node will be responsible for the whole physics simulations. Its main purpose is to carry the CelestialPhysicsController
    Node* CelestialPhysicsNode = new Node();
    CelestialPhysicsNode->SetController( controller );

    // A function to create stars for our simulation
    auto AddStar = [ & ]( float density, float mass, const Vector3D& position, Vector3D velocity )
    {
        float radius = Pow( 3.0f * mass / ( Math::four_pi * density ), 1.0f / 3.0f );

        auto star = new SphereGeometry( Vector3D( radius, radius, radius ) );

        star->SetNodePosition( Point3D( position ) );

        SphereGeometryObject* object = star->GetObject();

        object->SetCollisionExclusionMask( kCollisionExcludeAll );
        object->BuildPrimitive( star );
        object->SetGeometryFlags( object->GetGeometryFlags() | kGeometryDynamic );

        controller->AddBody( star, mass, velocity );

        AddNewNode( star );
    };


    AddStar( defaultStarSystemDensity, galacticCoreMass, Point3D( Point3D::zero ), Vector3D::zero );

    while ( num-- > 0 )
    {
        float    r = RandomFloat( 0.0f, 1.0f );
        Vector3D pos;
        float    mass;

        if ( r < 0.02f )
        { // Bulge of the galaxy
            float distanceFromCenter = galacticBulgeRadius * Pow( RandomFloat( 0.0f, 1.0f ), 2.0f );
            pos                      = distanceFromCenter * RandomUnitVector3D();
            mass                     = RandomFloat( 200.0f, 400.0f ); // heavier stars in core
        }
        else
        { // Main disk of the galaxy

            float distanceFromCenter = -galacticDiskRadius * Log( 1.0f - RandomFloat( 0.0f, 1.0f ) );
            float azimuth            = RandomFloat( 0.0f, Math::two_pi );
            pos  = Vector3D( Cos( azimuth ) * distanceFromCenter, Sin( azimuth ) * distanceFromCenter, RandomFloat( -0.02f, 0.02f ) );
            mass = 100.0f * Pow( RandomFloat( 0.0f, 1.0f ), 3.0f ) + 0.02f;
        }

        Vector3D velocity       = Vector3D::zero;
        float    planarDistance = Magnitude( pos.xy );

        if ( planarDistance > initalVelocityDistCuttoff )
        {
            Vector3D tangent( -pos.y, pos.x, 0.0f );
            tangent.Normalize();
            float orbitalSpeed = maxOrbitalSpeed * planarDistance / Sqrt( planarDistance * planarDistance + missingMassScaleRadiusSq );
            velocity           = tangent * orbitalSpeed;
        }

        AddStar( defaultStarSystemDensity, mass, pos, velocity );
    }

    AddNewNode( CelestialPhysicsNode );
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
