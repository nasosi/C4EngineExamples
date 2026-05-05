#pragma once


#include "C4Cameras.h"
#include "C4World.h"


using namespace C4;


class GameWorld : public World
{
private:

    SpectatorCamera spectatorCamera;
    void SetupSpectatorCamera();

public:

    GameWorld( const char* name );
    ~GameWorld() = default;

    WorldResult PreprocessWorld() override;

    SpectatorCamera* GetSpectatorCamera()
    {
        return ( &spectatorCamera );
    }
};

