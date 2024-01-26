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

class MovementAction : public Action
{
private:

    uint32      spectatorFlag;

public:

    MovementAction( ActionType type, uint32 specFlag);
    ~MovementAction();

    void HandleEngage( );
    void HandleDisengage( );
};


#endif //INPUT_HPP
