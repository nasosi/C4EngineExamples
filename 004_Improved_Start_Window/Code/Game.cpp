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

    // Set a dark background to match the start window UI.
    TheInterfaceMgr->SetThemeColor( kThemeColorDesktop, Color::black );

    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );
    TheInterfaceMgr->AddWidget( &startWin );

    // Override the Escape key to show the start window. Save the previous callback and
    // cookie so they can be restored on shutdown.
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

// Escape key callback: shows the start window. 'cookie' is expected to be a Game instance
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
