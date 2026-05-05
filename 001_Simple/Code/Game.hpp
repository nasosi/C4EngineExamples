#pragma once


#include "Input.hpp"

#include <C4Application.h>
#include <C4Markers.h>
#include <C4StringTable.h>


using namespace C4;


extern "C"
{
    // Entry point, C4Engine Engine will call this function to create an instance of your game
    C4_MODULE_EXPORT C4::Application* CreateApplication();
}


// Our game world will include a "Locator" so the code know where to place the camera when the game starts.
// Here we define its type to be a spectator.
enum : LocatorType
{
    kLocatorSpectator = 'spec'
};

class Game : public Application
{
private:

    // The string table includes various strings for our game. Check Import\Examples\001_Simple\Game\gameStrings.txt
    // For the simple example, we only need a string to indicate the "Locator" and "Spectator" strings to be used
    // in the C4 Engine interface. You can find them in the Object Tab under "Marker".
    StringTable stringTable;

    // We use this to register our specator locator with the engine
    LocatorRegistration spectatorLocatorRegistrar;

    // The various movement actions we support
    MovementAction forwardAction;
    MovementAction backwardAction;
    MovementAction leftAction;
    MovementAction rightAction;
    MovementAction upAction;
    MovementAction downAction;

public:
    Game();

    // The destructor will be used to clean up any resources we created, or other items
    // the engine requires.
    ~Game();

    // Function to make the World Manager create an instance of our custom world class 
    // ( GameWorld ) instead of the default, World class
    static World* CreateWorld( const char* name, void* cookie );
};
