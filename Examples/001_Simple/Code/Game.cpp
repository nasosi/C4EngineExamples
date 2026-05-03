#include "Game.hpp"
#include "AppInfo.hpp"
#include "World.hpp"

#include <C4ToolWindows.h>

// Entry point, C4Engine Engine will call this function to create an instance of your game
C4_MODULE_EXPORT C4::Application* CreateApplication()
{
    return new Game();
}

// Constructor for our application
// Custom controllers, script methods, models, input actions etc should be initialized and registered here.
Game::Game() :

    // The string table includes various strings for our game. Check Import\Examples\001_Simple\Game\gameStrings.txt
    // For the simple example, we only need a string to indicate the "Locator" and "Spectator" strings to be used
    // in the C4 Engine interface. You can find them in the Object Tab under "Marker".
    stringTable( APP_PATH "/Game/gameStrings" ),

    // We use this to register our specator locator with the engine, and in particular the 'spec' substring of the 'LOCA' entry.
    spectatorLocatorRegistrar( kLocatorSpectator, stringTable.GetString( StringID( 'LOCA', kLocatorSpectator ) ) ),

    // We create the actions we need to support for respondint to user input.
    forwardAction( kActionForward, kSpectatorMoveForward ),
    backwardAction( kActionBackward, kSpectatorMoveBackward ),
    leftAction( kActionLeft, kSpectatorMoveLeft ),
    rightAction( kActionRight, kSpectatorMoveRight ),
    upAction( kActionUp, kSpectatorMoveUp ),
    downAction( kActionDown, kSpectatorMoveDown )
{
    // Add mapped input actions to the Input Manager
    TheInputMgr->AddAction( &forwardAction );
    TheInputMgr->AddAction( &backwardAction );
    TheInputMgr->AddAction( &leftAction );
    TheInputMgr->AddAction( &rightAction );
    TheInputMgr->AddAction( &upAction );
    TheInputMgr->AddAction( &downAction );

    // Tell the world manager to create the world using our function CreateWorld
    TheWorldMgr->SetWorldCreator( &CreateWorld );

    // Tell the Interface Manager to automatically switch the input to the game
    // or gui depending on if a window is opened or not.
    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    Engine::Report( String<>( "Starting game: " ) + APPNAME );

    // Load the simple world
    // Comment the following line if you would prefer to manually load a world
    // from the console with the load command or from the Editor(using Ctrl+P).
    TheWorldMgr->LoadWorld( APP_PATH "/World/simple" );
}

Game::~Game()
{
    // Unload the current world if still active
    TheWorldMgr->UnloadWorld();
    TheWorldMgr->SetWorldCreator( nullptr );
}

// Returns a new instance of our custom class GameWorld that inherits from World
World* Game::CreateWorld( const char* name, void* cookie )
{
    return ( new GameWorld( name ) );
}
