#include "World.hpp"


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

WorldResult GameWorld::PreprocessWorld()
{
    WorldResult result = World::PreprocessWorld();
    if ( result != kWorldOkay )
    {
        return ( result );
    }

    SetupSpectatorCamera();

    return ( kWorldOkay );
}
