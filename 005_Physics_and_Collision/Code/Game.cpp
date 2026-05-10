#include "Game.hpp"
#include "World.hpp"

#include <C4ToolWindows.h>


C4_MODULE_EXPORT C4::Application* CreateApplication()
{
    return new Game();
}


Game::Game() :

    stringTable( "game/gameStrings" ),

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
    Game* game = static_cast<Game*>( cookie );

    if ( game )
    {
        game->startWin.ShowWidget();
    }
}


World* Game::CreateWorld( const char* name, void* /* cookie*/ )
{
    return new GameWorld( name );
}
