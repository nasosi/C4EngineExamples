#include "SpaceShipController.hpp"

#include "World.hpp"
#include "C4Engine.h"

float calculateCameraFocalLength(float fovAngle)
{
    int32 angle = Min(Max(fovAngle, 45), 90);
    return (1.0F / Tan(float(angle) * (Const::radians * 0.5F)));
}

GameWorld::GameWorld(const char *name) :
    World( name ),
    spectatorCamera( calculateCameraFocalLength( 65 ), 1.0F, 0.3F )
{
}

GameWorld::~GameWorld()
{
}

WorldResult GameWorld::PreprocessWorld(void)
{
    WorldResult result = World::PreprocessWorld();
    if (result != kWorldOkay)
    {
        return (result);
    }

    SetWorldCamera( &spectatorCamera );

    float azimuth = 0;
    float altitude = 0;
    Point3D position( 0.0f, 0.0f, 1.7f);

    Marker *marker = GetRootNode()->GetFirstMarker( );
    while ( marker )
    {
        if ((marker->GetMarkerType() == kMarkerLocator) && (marker->NodeEnabled()))
        {
            const auto locator = static_cast<const LocatorMarker *>(marker);
            if (locator->GetLocatorType() == 'spec')
            {
                const Vector3D direction = marker->GetWorldTransform()[0];
                azimuth       = Arctan( direction.y, direction.x );
                altitude      = Arctan( direction.z, Sqrt(direction.x * direction.x + direction.y * direction.y));
                position = marker->GetWorldPosition();
                break;
            }
        }

        marker = marker->GetNextListElement();
    }

    spectatorCamera.SetCameraAzimuth(azimuth);
    spectatorCamera.SetCameraAltitude(altitude);
    spectatorCamera.SetNodePosition(position);

    ManageSpaceShipNode();

    return (kWorldOkay);
}

void GameWorld::ManageSpaceShipNode()
{
    Node* rootNode = GetRootNode();
    Node* node = rootNode;

    while (node)
    {
        if (node->GetNodeType() == kNodeGeometry)
        {
            auto str = String<63>(node->GetNodeName());

            if (str == "SpaceShip")
            {
                Engine::Report(String<32>("Found ") + String<63> { node->GetNodeName() });

                auto spaceShip = static_cast<Geometry *>( node );

                auto controller = spaceShip->GetController();

                if (controller->GetControllerType() == kControllerSpaceShip)
                {
                    spaceShipController = static_cast<SpaceShipController*>(controller);
                    spaceShipController->SetRollRotationRateRadPerSec(0.0);
                }
                else
                {
                    Engine::Report(String<64>("Error: spaceship controller not found.") );
                }

                break;
            }
        }
        node = rootNode->GetNextTreeNode(node);
    }
}
