#ifndef GAME_HPP
#define GAME_HPP

#include "C4Application.h"
#include "C4Markers.h"
#include "C4StringTable.h"
#include "StartWindow.hpp"

#include "Input.hpp"

using namespace C4;

extern "C"
{
C4_MODULE_EXPORT C4::Application *CreateApplication( );
}

enum : LocatorType
{
    kLocatorSpectator   = 'spec'
};

class Game : public Application
{
private:
    StringTable         stringTable;

    LocatorRegistration spectatorLocatorRegistar;

    MovementAction  forwardAction;
    MovementAction  backwardAction;
    MovementAction  leftAction;
    MovementAction  rightAction;
    MovementAction  upAction;
    MovementAction  downAction;

    StartWindow     startWin;

public:
    Game();
    ~Game();

    static World *CreateWorld(const char *name, void *cookie);
};

#endif // GAME_HPP
