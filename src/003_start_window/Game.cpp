#include "C4ToolWindows.h"

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
    downAction      (kActionDown,       kSpectatorMoveDown )
{

    // Add mapped input actions to the Input Manager
    TheInputMgr->AddAction( &forwardAction );
    TheInputMgr->AddAction( &backwardAction );
    TheInputMgr->AddAction( &leftAction );
    TheInputMgr->AddAction( &rightAction );
    TheInputMgr->AddAction( &upAction );
    TheInputMgr->AddAction( &downAction );

    TheWorldMgr->SetWorldCreator( &CreateWorld );

    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    TheInterfaceMgr->AddWidget( &startWin );
}

Game::~Game()
{
    TheWorldMgr->UnloadWorld( );

    TheWorldMgr->SetWorldCreator( nullptr );
}

World *Game::CreateWorld(const char *name, void *cookie)
{
    return ( new GameWorld(name) );
}
