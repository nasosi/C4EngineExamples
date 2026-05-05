#include "Input.hpp"
#include "World.hpp"


MovementAction::MovementAction( ActionType type, uint32 specFlag ) : Action( type )
{
    spectatorFlag = specFlag;
}

// When the input is pressed, we enable the corresponding movement flag on the
// spectator camera. This does not directly move the camera; instead, it updates
// the camera state so that the world update step can apply movement consistently.
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

// When the input is released, we clear the corresponding movement flag.
// This ensures the camera stops applying that movement direction on the next update.
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
