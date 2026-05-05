#pragma once


#include "C4Input.h"


using namespace C4;


// Enums of the action types we want to support
enum : ActionType
{
    kActionForward  = 'frwd',
    kActionBackward = 'bkwd',
    kActionLeft     = 'left',
    kActionRight    = 'rght',
    kActionUp       = 'jump',
    kActionDown     = 'down',
};


// MovementAction is an input binding that modifies spectator camera movement state.
//
// Each instance represents a single movement direction (or behavior) and is
// parameterized by a spectatorFlag. When the action is engaged, that flag is
// added to the camera's spectator state; when disengaged, it is removed.
//
// This allows multiple movement inputs (e.g. left + forward) to be active
// simultaneously using a single bitmask stored in the camera.
//
// For your game to be interesting, you will likely other types of actions.
class MovementAction : public Action
{
private:

    uint32 spectatorFlag;

public:

    MovementAction( ActionType type, uint32 specFlag );
    ~MovementAction() = default;

    // Called when the action type is engaged
    void HandleEngage();

    // Called when the action type is disengaged
    void HandleDisengage();
};