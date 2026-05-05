#pragma once


#include "C4Cameras.h"
#include "C4World.h"


using namespace C4;


class SpaceShipController;


class GameWorld : public World
{
private:

    SpectatorCamera spectatorCamera;
    void            SetupSpectatorCamera();

    // The spaceship controller is assigned to a node in the world, and owned by that
    // node. We will keep a pointer to it here for access from the
    // SpaceShipControlActions (see Input.hpp).
    SpaceShipController* spaceShipController = nullptr;
    void                 SetupSpaceshipController();

public:

    GameWorld( const char* name );
    ~GameWorld() = default;

    WorldResult PreprocessWorld() override;

    SpectatorCamera* GetSpectatorCamera()
    {
        return &spectatorCamera;
    }

    const SpaceShipController* GetSpaceShipController() const
    {
        return spaceShipController;
    }

    SpaceShipController* GetSpaceShipController()
    {
        return spaceShipController;
    }
};
