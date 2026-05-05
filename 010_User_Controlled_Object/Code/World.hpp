#pragma once


#include "C4Cameras.h"
#include "C4World.h"


using namespace C4;


class SpaceshipController;


class GameWorld : public World
{
private:

    SpectatorCamera spectatorCamera;
    void            SetupSpectatorCamera();

    // The spaceship controller is assigned to a node in the world, and owned by that
    // node. We will keep a pointer to it here for access from the
    // SpaceshipControlActions (see Input.hpp).
    SpaceshipController* spaceShipController = nullptr;
    void                 SetupSpaceshipController();

public:

    GameWorld( const char* name );
    ~GameWorld() = default;

    WorldResult PreprocessWorld() override;

    SpectatorCamera* GetSpectatorCamera()
    {
        return &spectatorCamera;
    }

    const SpaceshipController* GetSpaceshipController() const
    {
        return spaceShipController;
    }

    SpaceshipController* GetSpaceshipController()
    {
        return spaceShipController;
    }
};
