// MIT License
//
// Copyright (c) 2023-2026 Athanasios Iliopoulos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// This software may contain work that is licensed under different terms. In that
// event, appropriate license files will be included in close folder proximity
// to said work. You need to ensure that your use of this software is compatible
// with the license of any other work that may be part of this software.

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
