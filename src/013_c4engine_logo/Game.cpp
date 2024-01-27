#include "Game.hpp"
#include "World.hpp"

#include "C4Logo.h"
#include "C4ToolWindows.h"

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

    controlKeysInfo( Vector2D( 420.0F, 32.0F ), "WASD to turn, space for throttle." )
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

    // Create the logo window and add it to the interface. It will delete itself upon logo replay completion
    LogoWindow* logoWindow = new LogoWindow;
    TheInterfaceMgr->AddWidget( logoWindow );

    // Call a function to perform extra setup steps after the logo has finished playing
    logoWindow->SetCompletionCallback( &Game::HandleC4EngineLogoCompletion, this );
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

// This function is called after the logo has finished playing
void Game::HandleC4EngineLogoCompletion( LogoWindow* window, void* cookie )
{
    Game* game = static_cast<Game*>( cookie );

    game->controlKeysInfo.SetFont( "font/Bold", 12 );
    game->controlKeysInfo.SetWidgetColor( ColorRGBA( 255.0f / 255.0f, 204.0f / 255.0f, 102.0f / 255.0f ) );
    game->controlKeysInfo.SetWidgetPosition( Point2D( 50.0f, 50.0f ) );

    TheInterfaceMgr->AddWidget( &game->controlKeysInfo );
}

World* Game::CreateWorld( const char* name, void* cookie )
{
    return ( new GameWorld( name ) );
}
