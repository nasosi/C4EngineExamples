#pragma once

#include "C4Input.h"

using namespace C4;

enum : ActionType
{
    kActionForward  = 'frwd',
    kActionBackward = 'bkwd',
    kActionLeft     = 'left',
    kActionRight    = 'rght',
    kActionUp       = 'jump',
    kActionDown     = 'down',
};

// Bitflags for the different motion states of the spaceship. Bitmasks are used so
// that we can combine multiple states at the same time (for example, accelerating 
// while pitching up and rolling left). The actual values of the flags don't matter.
enum
{
    kPitchUp                   = 1 << 0,
    kPitchDown                 = 1 << 1,
    kRollLeft                  = 1 << 2,
    kRollRight                 = 1 << 3,
    kYawLeft                   = 1 << 4,
    kYawRight                  = 1 << 5,
    kAccelerate                = 1 << 6,
    kDecelerate                = 1 << 7
};

// We don't plan to dynamically update the type and flag parameters, so we can use a
// template to bind them at compile time.
template <ActionType type, uint32 Flag>
class SpaceShipControlAction : public Action
{
public:

    SpaceShipControlAction() : Action( type )
    {
    }

    ~SpaceShipControlAction()
    {
    }

    void HandleEngage()
    {
        GameWorld* world = static_cast<GameWorld*>( TheWorldMgr->GetWorld() );
        if ( world )
        {
            world->spaceShipController->motionFlags |= Flag;
        }
    }

    void HandleDisengage()
    {
        GameWorld* world = static_cast<GameWorld*>( TheWorldMgr->GetWorld() );
        if ( world )
        {
            world->spaceShipController->motionFlags &= ~Flag;
        }
    }
};
