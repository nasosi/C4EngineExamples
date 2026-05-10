#include "Game.hpp"
#include "World.hpp"
#include "AppInfo.hpp"

#include <C4ToolWindows.h>


C4_MODULE_EXPORT C4::Application* CreateApplication()
{
   return new Game( );
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

    // Request The Interface Manager to switch input modes automatically:
    // If there is no active UI window: game input is enabled
    // If there is an active UI window: input is routed to the UI
    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    // We add the start window to the interface manager here so that it will be 
    // displayed before any world is loaded. Once the start button is clicked, the 
    // start window will hide itself and the world will be loaded. Note that the start
    // window is displayed only once here. The Improved Start Window examples fixes
    // this.
    TheInterfaceMgr->AddWidget( &startWin );
}


Game::~Game()
{
    TheWorldMgr->UnloadWorld();

    TheWorldMgr->SetWorldCreator( nullptr );
}


World* Game::CreateWorld( const char* name, void* cookie )
{
    return ( new GameWorld( name ) );
}
