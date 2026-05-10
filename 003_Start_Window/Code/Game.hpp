#pragma once


#include "Input.hpp"
#include "StartWindow.hpp"

#include <C4Application.h>
#include <C4Markers.h>
#include <C4StringTable.h>
#include <C4ToolWindows.h>


using namespace C4;


extern "C"
{
    C4_MODULE_EXPORT C4::Application* CreateApplication();
}


enum : LocatorType
{
    kLocatorSpectator = 'spec'
};


class Game : public Application
{
private:
    StringTable stringTable;

    LocatorRegistration spectatorLocatorRegistrar;

    MovementAction forwardAction;
    MovementAction backwardAction;
    MovementAction leftAction;
    MovementAction rightAction;
    MovementAction upAction;
    MovementAction downAction;

    // Our start window
    StartWindow startWin;

public:
    Game();
    ~Game();

    static World* CreateWorld( const char* name, void* cookie );
};