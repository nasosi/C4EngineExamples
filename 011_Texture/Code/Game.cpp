#include "Game.hpp"
#include "World.hpp"

#include "AppInfo.hpp"


C4_MODULE_EXPORT C4::Application* CreateApplication()
{
    return new Game();
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

    TheInterfaceMgr->SetInputManagementMode( kInputManagementAutomatic );

    TheWorldMgr->SetWorldCreator( &CreateWorld );
    TheWorldMgr->LoadWorld( APP_PATH "/World/011_Texture" );
}


Game::~Game()
{
    TheWorldMgr->UnloadWorld();
    TheWorldMgr->SetWorldCreator( nullptr );
}


World* Game::CreateWorld( const char* name, void* cookie )
{
    return new GameWorld( name );
}
