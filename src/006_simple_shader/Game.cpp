#include "C4ToolWindows.h"

#include "Game.hpp"
#include "World.hpp"

C4_MODULE_EXPORT C4::Application* CreateApplication()
{
    return new ( Game );
}

Game::Game() :
    stringTable( APPNAME "/Game/gameStrings" ),

    spectatorLocatorRegistar( kLocatorSpectator, stringTable.GetString( StringID( 'LOCA', kLocatorSpectator ) ) ),

    forwardAction( kActionForward, kSpectatorMoveForward ),
    backwardAction( kActionBackward, kSpectatorMoveBackward ),
    leftAction( kActionLeft, kSpectatorMoveLeft ),
    rightAction( kActionRight, kSpectatorMoveRight ),
    upAction( kActionUp, kSpectatorMoveUp ),
    downAction( kActionDown, kSpectatorMoveDown )
{
    // Set the screen background to black
    TheEngine->GetVariable( "desktopColor" )->SetValue( "000000FF" );

    TheInputMgr->AddAction( &forwardAction );
    TheInputMgr->AddAction( &backwardAction );
    TheInputMgr->AddAction( &leftAction );
    TheInputMgr->AddAction( &rightAction );
    TheInputMgr->AddAction( &upAction );
    TheInputMgr->AddAction( &downAction );

    TheWorldMgr->SetWorldCreator( &CreateWorld );

    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    TheInterfaceMgr->AddWidget( &startWin );

    // We store the current escape process and cookie to assign it again when
    // we dont need it. We also se the escape process to our function
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

// We handle escape by showing the start window
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
