#include "Game.hpp"
#include "World.hpp"

#include "C4ToolWindows.h"



C4_MODULE_EXPORT C4::Application * CreateApplication( )
{
    return new( Game );
}

// Constructor for our application
// Custom controllers, script methods, models, input actions etc should be initialized and registered here.
Game::Game() :
    // Create a string that can be associated from strings.txt
    stringTable( APPNAME "/Game/gameStrings"),
    spectatorLocatorRegistar(kLocatorSpectator, stringTable.GetString(StringID('LOCA', kLocatorSpectator))),
    
    forwardAction   (kActionForward,    kSpectatorMoveForward ),
    backwardAction  (kActionBackward,   kSpectatorMoveBackward) ,
    leftAction      (kActionLeft,       kSpectatorMoveLeft ),
    rightAction     (kActionRight,      kSpectatorMoveRight ),
    upAction        (kActionUp,         kSpectatorMoveUp ),
    downAction      (kActionDown,       kSpectatorMoveDown )
{
    // Add mapped input actions to the Input Manager
    TheInputMgr->AddAction( &forwardAction );
    TheInputMgr->AddAction( &backwardAction );
    TheInputMgr->AddAction( &leftAction );
    TheInputMgr->AddAction( &rightAction );
    TheInputMgr->AddAction( &upAction );
    TheInputMgr->AddAction( &downAction );

    // Tell the world manager to create the worlds using our function CreateWorld
    TheWorldMgr->SetWorldCreator( &CreateWorld );

    // Tells the Interface Manager to automatically switch the input to the game
    // or gui depending on if a window is opened or not.
    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    Engine::Report(APPNAME);

    // Load the world
    // Comment the following line if you would prefer to manually load a world
    // from the console with the load command or from the Editor(using Ctrl+P).
    TheWorldMgr->LoadWorld(APPNAME "/World/013_terrain");
}

Game::~Game()
{
    // Unload the current world if still active
    TheWorldMgr->UnloadWorld( );

    TheWorldMgr->SetWorldCreator(nullptr);
}

// Returns a new instance of our custom class GameWorld that inherits from World
World *Game::CreateWorld(const char *name, void *cookie)
{
    return ( new GameWorld(name) );
}
