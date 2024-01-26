#ifndef INPUT_HPP
#define INPUT_HPP

#include "C4Input.h"

using namespace C4;

enum : ActionType
{
    kActionForward = 'frwd',
    kActionBackward = 'bkwd',
    kActionLeft = 'left',
    kActionRight = 'rght',
    kActionUp = 'jump',
    kActionDown = 'down',
};

enum
{
    kPitchUp    = 1 << 0,
    kPitchDown  = 1 << 1,
    kRollLeft   = 1 << 2,
    kRollRight  = 1 << 3,
    kYawLeft    = 1 << 4,
    kYawRight   = 1 << 5,
    kAccelerate = 1 << 6,
    kDecelerate = 1 << 7,
    kSpaceShipMotionPlanarMask = 15
};


template <uint32 Flag>
class SpaceShipControlAction : public Action
{
public:
    SpaceShipControlAction( ActionType type ) : Action( type ) { }
    ~SpaceShipControlAction( ) { }

    void HandleEngage() 
    {
        GameWorld* world = static_cast<GameWorld*>(TheWorldMgr->GetWorld());
        if (world)
        {
            world->spaceShipController->motionFlags |= Flag;
        }
    }

    void HandleDisengage( )
    {
        GameWorld* world = static_cast<GameWorld*>(TheWorldMgr->GetWorld());
        if (world)
        {
            world->spaceShipController->motionFlags &= ~Flag;
        }
    }
};



#endif //INPUT_HPP
