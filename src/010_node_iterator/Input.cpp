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
