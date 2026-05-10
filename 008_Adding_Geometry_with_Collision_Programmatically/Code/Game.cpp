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

#include "C4ToolWindows.h"
#include "World.hpp"


C4_MODULE_EXPORT C4::Application* CreateApplication()
{
    return new Game();
}

Game::Game() :
    stringTable( APP_PATH "/Game/gameStrings" ),
    spectatorLocatorRegistrar( kLocatorSpectator, stringTable.GetString( StringID( 'LOCA', kLocatorSpectator ) ) ),

    forwardAction( kActionForward, kSpectatorMoveForward ),
    backwardAction( kActionBackward, kSpectatorMoveBackward ),
    leftAction( kActionLeft, kSpectatorMoveLeft ),
    rightAction( kActionRight, kSpectatorMoveRight ),
    upAction( kActionUp, kSpectatorMoveUp ),
    downAction( kActionDown, kSpectatorMoveDown )
{
    TheInputMgr->AddAction( &forwardAction );
    TheInputMgr->AddAction( &backwardAction );
    TheInputMgr->AddAction( &leftAction );
    TheInputMgr->AddAction( &rightAction );
    TheInputMgr->AddAction( &upAction );
    TheInputMgr->AddAction( &downAction );

    TheWorldMgr->SetWorldCreator( &CreateWorld );

    TheInterfaceMgr->SetThemeColor( kThemeColorDesktop, Color::black );
    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );
    TheInterfaceMgr->AddWidget( &startWin );

    previousEscapeCallback = TheInputMgr->GetEscapeCallback();
    previousEscapeCookie   = TheInputMgr->GetEscapeCookie();
    TheInputMgr->SetEscapeCallback( &HandleEscape, this );
}

Game::~Game()
{
    TheWorldMgr->UnloadWorld();

    TheWorldMgr->SetWorldCreator( nullptr );

    TheInputMgr->SetEscapeCallback( previousEscapeCallback, previousEscapeCookie );
}

void Game::HandleEscape( void* cookie )
{
    auto instance = static_cast<Game*>( cookie );

    if ( instance )
    {
        instance->startWin.ShowWidget();
    }
}

World* Game::CreateWorld( const char* name, void* cookie )
{
    return ( new GameWorld( name ) );
}
