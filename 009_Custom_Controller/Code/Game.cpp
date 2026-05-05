#include "Game.hpp"
#include "AppInfo.hpp"
#include "World.hpp"

#include <C4ToolWindows.h>


// This is the  Game class global. We initialize to nullptr and the Game class 
// constructor will set it when the Engine loads our game dynamic library.
Game* TheGame = nullptr;


C4_MODULE_EXPORT C4::Application* CreateApplication()
{
   return new Game( );
}

Game::Game() :
    // We initialize the global pointer to the current game instance first.
    Global<Game>( TheGame ),

    stringTable( "Examples/" APPNAME "/Game/gameStrings" ),

    spectatorLocatorRegistrar( kLocatorSpectator, stringTable.GetString( StringID( 'LOCA', kLocatorSpectator ) ) ),

    rotateControllerReg( kControllerRotate, stringTable.GetString( StringID( 'ctls', 'rctl' ) ) ),

    forwardAction( kActionForward, kSpectatorMoveForward ),
    backwardAction( kActionBackward, kSpectatorMoveBackward ),
    leftAction( kActionLeft, kSpectatorMoveLeft ),
    rightAction( kActionRight, kSpectatorMoveRight ),
    upAction( kActionUp, kSpectatorMoveUp ),
    downAction( kActionDown, kSpectatorMoveDown )
{
    TheInterfaceMgr->SetThemeColor( kThemeColorDesktop, Color::black );

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
