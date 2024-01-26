#ifndef WORLD_HPP
#define WORLD_HPP

#include "C4World.h"
#include "C4Cameras.h"

using namespace C4;

class SpaceShipController;

class GameWorld : public World
{
private:

    SpectatorCamera   spectatorCamera;

public:
    GameWorld( const char *name );
    ~GameWorld( );

    WorldResult PreprocessWorld( ) override;

    void ManageSpaceShipNode();

    SpectatorCamera *GetSpectatorCamera( )
    {
        return ( &spectatorCamera );
    }

    SpaceShipController* spaceShipController = nullptr;

};

#endif // WORLD_HPP
