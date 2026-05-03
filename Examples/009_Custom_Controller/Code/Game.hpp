#pragma once


#include "Input.hpp"
#include "RotateController.hpp"
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


// Because we want to access the game's string table from the Rotate Controller to 
// grab some strings, we make the Game class inherit from Global<Game>. This allows
// us to access the single instance of the Game class from anywhere in the code 
// using TheGame pointer. See end of this file for the declaration of TheGame pointer.
class Game : public Application, public Global<Game> 
{
    private:

    StringTable stringTable;

    LocatorRegistration spectatorLocatorRegistrar;

    // We create a registar so we can register the controller with the engine
    // This happens in the constructor of our Game class
    ControllerReg<RotateController> rotateControllerReg;

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

public:

    Game();
    ~Game();

    static World* CreateWorld( const char* name, void* cookie );

    const StringTable& GetStringTable() const
    {
        return stringTable;
    }
};

// A pointer to the single instance of the Game so that any other part of the game 
// dll can access it.
extern Game* TheGame;
