#include "World.hpp"
#include "AppInfo.hpp"

#include <C4Engine.h> // For Engine::Report:

// Some default constants for our spectator camera. These will be used if no 'spec' locator is defined in the world.
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
    // Set the active camera to our spectator camera
    SetWorldCamera( &spectatorCamera );

    // Default camera setting if no spectator locator is defined in the world
    float   azimuth  = 0;
    float   altitude = 0;
    Point3D position = cameraDefaultPosition;

    // Traverse the scene and check for a 'spec' marker ( Locator Marker with type kMarkerLocator )
    // This should have been defined in the world
    Marker* marker = GetRootNode()->GetFirstMarker();
    while ( marker )
    {
        if ( ( marker->GetMarkerType() == kMarkerLocator ) && ( marker->NodeEnabled() ) )
        {
            const auto locator = static_cast<const LocatorMarker*>( marker );
            if ( locator->GetLocatorType() == 'spec' )
            {
                // Ok, we found it. Position and orient our camera using this marker.
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


WorldResult GameWorld::PreprocessWorld()
{
    // This will display a message in the console. Access it by pressing "~" on the keyboard
    Engine::Report( String<>( "Pre-processing world!" ) );


    WorldResult result = World::PreprocessWorld();
    if ( result != kWorldOkay )
    {
        return ( result );
    }

    // Make sure we have a camera that will be used to render the world. In this case,
    // we will use our spectator camera as the active camera for the world.
    SetupSpectatorCamera();

    return kWorldOkay;
}
