#ifndef GAME_HPP
#define GAME_HPP

#include "Input.hpp"

#include "C4Application.h"
#include "C4Markers.h"
#include "C4StringTable.h"

using namespace C4;

extern "C"
{
    // Entry point, C4Engine Engine will call this function to create an instance of your application(game)
    C4_MODULE_EXPORT C4::Application* CreateApplication();
}

enum : LocatorType
{
    kLocatorSpectator = 'spec'
};

class Game : public Application
{
private:
    StringTable stringTable;

    LocatorRegistration spectatorLocatorRegistar;

    MovementAction forwardAction;
    MovementAction backwardAction;
    MovementAction leftAction;
    MovementAction rightAction;
    MovementAction upAction;
    MovementAction downAction;

public:
    Game();
    ~Game() override;

    // Function to make the World Manager create an instance of our custom world
    // class(GameWorld) instead of the default, World class
    static World* CreateWorld( const char* name, void* cookie );
};

#endif // GAME_HPP
