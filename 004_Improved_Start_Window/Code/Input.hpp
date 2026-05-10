#pragma once


#include <C4Input.h>


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


class MovementAction : public Action
{
private:

    uint32 spectatorFlag;

public:

    MovementAction( ActionType type, uint32 specFlag );
    ~MovementAction() = default;

    void HandleEngage();
    void HandleDisengage();
};
