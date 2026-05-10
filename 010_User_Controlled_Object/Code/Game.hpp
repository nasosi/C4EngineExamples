#pragma once


#include "Input.hpp"
#include "SpaceshipController.hpp"
#include "StartWindow.hpp"


#include "C4Markers.h"
#include "C4StringTable.h"
#include <C4Application.h>


using namespace C4;


extern "C"
{
    C4_MODULE_EXPORT C4::Application* CreateApplication( void );
}


enum : LocatorType
{
    kLocatorSpectator = 'spec'
};


class Game : public Application, Global<Game>
{
private:

    StringTable stringTable;

    LocatorRegistration spectatorLocatorRegistrar;

    // We create a registar so we can register the spaceship controller with the engine
    ControllerReg<SpaceshipController> rotateControllerReg;

    // Here we define the different input actions for controlling the spaceship, using
    // our template-based SpaceshipControlAction to bind the input type and motion flag
    // at compile time.
    SpaceshipControlAction<kActionLeft, kRollLeft>      rollLeft;
    SpaceshipControlAction<kActionRight, kRollRight>    rollRight;
    SpaceshipControlAction<kActionForward, kPitchUp>    pitchUp;
    SpaceshipControlAction<kActionBackward, kPitchDown> pitchDown;
    SpaceshipControlAction<kActionUp, kAccelerate>      accelerate;
    SpaceshipControlAction<kActionDown, kDecelerate>    decelerate;

    StartWindow startWin;

    InputMgr::KeyCallback* previousEscapeCallback;
    void*                  previousEscapeCookie;

    static void HandleEscape( void* cookie );

public:

    Game();
    ~Game();

    static World* CreateWorld( const char* name, void* cookie );

    const StringTable& GetStringTable() const
    {
        return stringTable;
    }
};


extern Game* TheGame;
