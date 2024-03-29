#include "World.hpp"
#include "BoxesGrid.hpp"

#include <C4Engine.h>

float calculateCameraFocalLength( float fovAngle )
{
    int32 angle = Min( Max( fovAngle, 45 ), 90 );
    return ( 1.0F / Tan( float( angle ) * ( Const::radians * 0.5F ) ) );
}

GameWorld::GameWorld( const char* name ) : World( name ), spectatorCamera( calculateCameraFocalLength( 65 ), 1.0F, 0.3F )
{
}

GameWorld::~GameWorld()
{
}

WorldResult GameWorld::PreprocessWorld()
{
    WorldResult result = World::PreprocessWorld();
    if ( result != kWorldOkay )
    {
        return ( result );
    }

    SetWorldCamera( &spectatorCamera );

    float   azimuth  = 0;
    float   altitude = 0;
    Point3D position( 0.0f, 0.0f, 1.7f );

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

    addBoxesGridToTheWorld( this );

    // Save our world so we can manipulate it in the world editor if desired
    File       file;
    auto       fname   = String<>( "Data/Application/" APPNAME "/World/simple_physics_prgrm_out.wld" );
    FileResult fResult = file.OpenFile( fname, kFileCreate );
    if ( fResult == kFileOkay )
    {
        Engine::Report( String<>( "Saving to: " ) + fname );
        GetRootNode()->PackWorldResource( file, kPackInitialize );
    }
    else
    {
        Engine::Report( String<>( "Could not open file for writing: " ) + fname );
    }

    return ( kWorldOkay );
}
