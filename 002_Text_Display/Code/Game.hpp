#pragma once


#include "Input.hpp"


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

    // A text widget that will hold the text
    TextWidget text;

public:
    Game();
    ~Game();

    static World* CreateWorld( const char* name, void* cookie );
};
