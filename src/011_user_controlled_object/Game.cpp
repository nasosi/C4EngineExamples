#include "C4ToolWindows.h"

#include "Game.hpp"
#include "World.hpp"

C4_MODULE_EXPORT C4::Application* CreateApplication( void )
{
    return new ( Game );
}

Game::Game() :
    stringTable( APPNAME "/Game/gameStrings" ),

    spectatorLocatorRegistar( kLocatorSpectator, stringTable.GetString( StringID( 'LOCA', kLocatorSpectator ) ) ),

    rotateControllerReg( kControllerSpaceShip, "SpaceShip Controller" ),

    rollLeft( kActionLeft ),
    rollRight( kActionRight ),
    pitchUp( kActionForward ),
    pitchDown( kActionBackward ),
    accelerate( kActionUp ),
    decelerate( kActionDown ),

    controlKeysInfo( Vector2D( 480.0F, 32.0F ), "'WASD' to turn, 'space' to accelerate, 'c' to deccelerate." )
{
    TheInputMgr->AddAction( &rollLeft );
    TheInputMgr->AddAction( &rollRight );
    TheInputMgr->AddAction( &pitchUp );
    TheInputMgr->AddAction( &pitchDown );
    TheInputMgr->AddAction( &accelerate );
    TheInputMgr->AddAction( &decelerate );

    TheWorldMgr->SetWorldCreator( &CreateWorld );

    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    TheInterfaceMgr->AddWidget( &startWin );

    // We store the current escape process and cookie to assign it again when
    // we dont need it. We also se the escape process to our function
    previousEscapeCallback = TheInputMgr->GetEscapeCallback();
    previousEscapeCookie   = TheInputMgr->GetEscapeCookie();
    TheInputMgr->SetEscapeCallback( &HandleEscape, this );

    controlKeysInfo.SetFont( "font/Bold", 12 );
    controlKeysInfo.SetWidgetColor( ColorRGBA( 255.0f / 255.0f, 204.0f / 255.0f, 102.0f / 255.0f ) );
    controlKeysInfo.SetWidgetPosition( Point2D( 25.0f, 25.0f ) );

    TheInterfaceMgr->AddWidget( &controlKeysInfo );
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
    auto c = static_cast<Game*>( cookie );

    c->startWin.ShowWidget();
}

World* Game::CreateWorld( const char* name, void* cookie )
{
    return ( new GameWorld( name ) );
}
