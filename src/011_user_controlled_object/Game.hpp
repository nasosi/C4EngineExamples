#ifndef GAME_HPP
#define GAME_HPP

#include "Input.hpp"
#include "SpaceShipController.hpp"
#include "StartWindow.hpp"

#include "C4Application.h"
#include "C4Markers.h"
#include "C4StringTable.h"

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

    ~Game() override;

    static World* CreateWorld( const char* name, void* cookie );

private:
    StringTable stringTable;

    TextWidget controlKeysInfo;

    LocatorRegistration spectatorLocatorRegistar;

    // We create a registar so we can register the controller with the engine
    // This happens in the constructor of our Game class
    ControllerReg<SpaceShipController> rotateControllerReg;

    SpaceShipControlAction<kRollLeft>   rollLeft;
    SpaceShipControlAction<kRollRight>  rollRight;
    SpaceShipControlAction<kPitchUp>    pitchUp;
    SpaceShipControlAction<kPitchDown>  pitchDown;
    SpaceShipControlAction<kAccelerate> accelerate;
    SpaceShipControlAction<kDecelerate> decelerate;

    StartWindow startWin;

    // We store a reference to the previous escape process and cookie to restore
    // when not needed anymore.
    InputMgr::KeyCallback* previousEscapeCallback;
    void*                  previousEscapeCookie;

    // This is how we handle when the user clicks escape
    static void HandleEscape( void* cookie );
};

#endif // GAME_HPP
