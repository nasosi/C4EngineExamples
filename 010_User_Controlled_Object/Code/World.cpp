#include "World.hpp"
#include "Game.hpp"
#include "SpaceshipController.hpp"

#include <C4Engine.h>


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


WorldResult GameWorld::PreprocessWorld( void )
{
    WorldResult result = World::PreprocessWorld();
    if ( result != kWorldOkay )
    {
        return ( result );
    }

    SetupSpectatorCamera();

    SetupSpaceshipController();

    return kWorldOkay;
}


void GameWorld::SetupSpaceshipController()
{
    // Find and cache the spaceship controller in the world.
    
    // We start by clearing our cached pointer to ensure we don’t accidentally use
    // a stale controller from a previous world or setup step.
    spaceShipController = nullptr;

    // Traverse the entire scene graph starting from the root node C4 worlds are 
    // organized as a tree, so we iterate over every node using GetNextTreeNode().
    Node* rootNode = GetRootNode();
    Node* node     = rootNode;
    while ( node )
    {
        // We’re only interested in enabled geometry nodes, since controllers are typically 
        // attached to Geometry objects.
        if ( node->NodeEnabled() && node->GetNodeType() == kNodeGeometry )
        {
            // Safe to cast because we just verified the node type.
            auto geom       = static_cast<Geometry*>( node );
            auto controller = geom->GetController();

            // Check whether this geometry has a controller, and if so, whether it’s the 
            // spaceship controller type we’re looking for.
            if ( controller && controller->GetControllerType() == kControllerSpaceship )
            {
                // Cache the controller for later use.
                spaceShipController = static_cast<SpaceshipController*>( controller );
                spaceShipController->SetRollRate( 0.0 );

                // Stop searching once the first matching controller is found. This assumes there 
                // is only one spaceship controller in the world.
                break;
            }
        }

        // Advance to the next node in the scene graph. Notice that we are using the root 
        // node to get the next tree node, which ensures we traverse the entire hierarchy.
        node = rootNode->GetNextTreeNode( node );
    }

    // If no spaceship controller was found, log a warning to help catch setup issues 
    // in the world/level data.
    if ( spaceShipController == nullptr )
    {
        Engine::Report( "Warning: spaceship controller not found in world." );
    }
}
