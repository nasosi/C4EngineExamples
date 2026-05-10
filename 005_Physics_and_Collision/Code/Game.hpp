#pragma once

#include "Input.hpp"
#include "StartWindow.hpp"

#include <C4Application.h>
#include <C4Markers.h>
#include <C4StringTable.h>


using namespace C4;


extern "C"
{
    C4_MODULE_EXPORT C4::Application* CreateApplication( void );
}


enum : LocatorType
{
    kLocatorSpectator = 'spec'
};


class Game : public Application
{
public:

    Game();
    ~Game();

    static World* CreateWorld( const char* name, void* cookie );

private:

    StringTable stringTable;

    LocatorRegistration spectatorLocatorRegistrar;

    MovementAction forwardAction;
    MovementAction backwardAction;
    MovementAction leftAction;
    MovementAction rightAction;
    MovementAction upAction;
    MovementAction downAction;

    StartWindow startWin;

    InputMgr::KeyCallback* previousEscapeCallback;
    void*                  previousEscapeCookie;

    static void HandleEscape( void* cookie );
};
