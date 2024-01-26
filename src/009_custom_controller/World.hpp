#ifndef WORLD_HPP
#define WORLD_HPP

#include "C4Cameras.h"
#include "C4World.h"

using namespace C4;

class GameWorld : public World
{
private:
    SpectatorCamera spectatorCamera;

public:
    GameWorld( const char* name );
    ~GameWorld();

    WorldResult PreprocessWorld() override;

    SpectatorCamera* GetSpectatorCamera()
    {
        return ( &spectatorCamera );
    }
};

#endif // WORLD_HPP
