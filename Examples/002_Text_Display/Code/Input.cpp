#include "Input.hpp"
#include "World.hpp"


MovementAction::MovementAction( ActionType type, uint32 specFlag ) : Action( type )
{
    spectatorFlag = specFlag;
}


void MovementAction::HandleEngage()
{
    GameWorld* world = static_cast<GameWorld*>( TheWorldMgr->GetWorld() );
    if ( world )
    {
        auto camera = world->GetSpectatorCamera();

        // Enable this movement mode by setting the corresponding bit in the
        // spectator flag mask.
        camera->SetSpectatorFlags( camera->GetSpectatorFlags() | spectatorFlag );
    }
}


void MovementAction::HandleDisengage()
{
    auto world = static_cast<GameWorld*>( TheWorldMgr->GetWorld() );
    if ( world )
    {
        auto camera = world->GetSpectatorCamera();

        // Remove this movement mode by clearing its bit in the flag mask.
        camera->SetSpectatorFlags( camera->GetSpectatorFlags() & ~spectatorFlag );
    }
}
