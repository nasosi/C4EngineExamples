#include "Game.hpp"
#include "World.hpp"

C4_MODULE_EXPORT C4::Application * CreateApplication( )
{
    return new( Game );
}

Game::Game() :
    stringTable( APPNAME "/Game/gameStrings"),
    spectatorLocatorRegistar(kLocatorSpectator, stringTable.GetString(StringID('LOCA', kLocatorSpectator))),

    forwardAction   (kActionForward,    kSpectatorMoveForward ),
    backwardAction  (kActionBackward,   kSpectatorMoveBackward) ,
    leftAction      (kActionLeft,       kSpectatorMoveLeft ),
    rightAction     (kActionRight,      kSpectatorMoveRight ),
    upAction        (kActionUp,         kSpectatorMoveUp ),
    downAction      (kActionDown,       kSpectatorMoveDown ),

    text( Vector2D(240.0F, 32.0F), "Example Text." )
{
    TheInputMgr->AddAction( &forwardAction );
    TheInputMgr->AddAction( &backwardAction );
    TheInputMgr->AddAction( &leftAction );
    TheInputMgr->AddAction( &rightAction );
    TheInputMgr->AddAction( &upAction );
    TheInputMgr->AddAction( &downAction );

    TheWorldMgr->SetWorldCreator( &CreateWorld );

    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    // Set text widget properties and add it to the interface
    text.SetFont("font/Bold", 24);
    text.SetWidgetColor(ColorRGBA( 255.0f/255.0f, 204.0f/255.0f, 102.0f/255.0f));
    text.SetWidgetPosition(Point2D(50.0f, 50.0f));
    TheInterfaceMgr->AddWidget( &text );

    TheWorldMgr->LoadWorld( APPNAME "/World/simple");
}

Game::~Game()
{
    TheWorldMgr->UnloadWorld( );

    TheWorldMgr->SetWorldCreator(nullptr);
}

World *Game::CreateWorld(const char *name, void *cookie)
{
    return ( new GameWorld(name) );
}
