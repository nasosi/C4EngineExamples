#include "Game.hpp"
#include "AppInfo.hpp"
#include "World.hpp"


#include <C4ToolWindows.h>


Game* TheGame = nullptr;


C4_MODULE_EXPORT C4::Application* CreateApplication( void )
{
    return new Game();
}


Game::Game() :

    Global<Game>( TheGame ),

    stringTable( APP_PATH "/Game/gameStrings" ),

    spectatorLocatorRegistrar( kLocatorSpectator, stringTable.GetString( StringID( 'LOCA', kLocatorSpectator ) ) ),

    // 'ssct' is the string ID used to retrieve the "Spaceship Controller" display name.
    //
    // This string is shown in the world editor UI for the controller. It is defined in
    // gameStrings.txt (Import folder) and compiled into gameStrings.str in the Data
    // folder when the developer imports it from within the C4 Engine editor.
    rotateControllerReg( kControllerSpaceShip, stringTable.GetString( StringID( 'ctls', 'ssct' ) ) )
{
    TheWorldMgr->SetWorldCreator( &CreateWorld );

    TheInputMgr->AddAction( &rollLeft );
    TheInputMgr->AddAction( &rollRight );
    TheInputMgr->AddAction( &pitchUp );
    TheInputMgr->AddAction( &pitchDown );
    TheInputMgr->AddAction( &accelerate );
    TheInputMgr->AddAction( &decelerate );

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
    return new GameWorld( name );
}