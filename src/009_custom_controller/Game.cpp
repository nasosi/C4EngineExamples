#include "C4ToolWindows.h"

#include "Game.hpp"
#include "World.hpp"

C4_MODULE_EXPORT C4::Application* CreateApplication()
{
    return new ( Game );
}

Game::Game() :
    stringTable( "game/gameStrings" ),

    spectatorLocatorRegistar( kLocatorSpectator, stringTable.GetString( StringID( 'LOCA', kLocatorSpectator ) ) ),

    rotateControllerReg( kControllerRotate, "Rotate Controller" ),

    forwardAction( kActionForward, kSpectatorMoveForward ),
    backwardAction( kActionBackward, kSpectatorMoveBackward ),
    leftAction( kActionLeft, kSpectatorMoveLeft ),
    rightAction( kActionRight, kSpectatorMoveRight ),
    upAction( kActionUp, kSpectatorMoveUp ),
    downAction( kActionDown, kSpectatorMoveDown )
{
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
