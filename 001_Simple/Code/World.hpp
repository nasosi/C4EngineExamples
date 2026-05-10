#pragma once


#include "C4Cameras.h"
#include "C4World.h"


using namespace C4;


class GameWorld : public World
{
private:

    // This is our spectator camera, which we will use as the active camera for the world.
    SpectatorCamera spectatorCamera;
    void            SetupSpectatorCamera();

public:

    // The World constructor takes the file name of the world resource to load.
    GameWorld( const char* name );

    // The destructor is a virtual function, but we don't need to do anything special
    // in it, so we can use the default implementation.
    ~GameWorld() = default;

    // Preprocess functionallity when the world is loaded.
    WorldResult PreprocessWorld() override;

    SpectatorCamera* GetSpectatorCamera()
    {
        return ( &spectatorCamera );
    }
};