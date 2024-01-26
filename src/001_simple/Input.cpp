#include "Input.hpp"
#include "World.hpp"

MovementAction::MovementAction( ActionType type, uint32 specFlag) :
    Action( type )
{
    spectatorFlag = specFlag;
}

MovementAction::~MovementAction()
{
}

void MovementAction::HandleEngage( )
{
    GameWorld *world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
    if (world)
    {
        auto camera = world->GetSpectatorCamera( );
        // We want to set the spectator flag of the particular action so that
        // the camera knows to move to the right direction when called by the
        // engine. For example if spectatorFlag = kSpectatorMoveLeft this will 
        // enable moving to the left as long as the "left" button is Engaged:
        camera->SetSpectatorFlags(camera->GetSpectatorFlags() | spectatorFlag);
    }
}

void MovementAction::HandleDisengage( )
{
    auto world = static_cast<GameWorld *>(TheWorldMgr->GetWorld());
    if (world)
    {
        auto camera = world->GetSpectatorCamera();
        camera->SetSpectatorFlags(camera->GetSpectatorFlags() & ~spectatorFlag);
    }
}
