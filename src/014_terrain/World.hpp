#ifndef WORLD_HPP
#define WORLD_HPP

#include "C4Cameras.h"
#include "C4World.h"

using namespace C4;

class GameWorld : public World
{
private:
    // Cameras are not defined in the World Editor. Instead Locators are. We
    // will use a special locator to set the camera in the PreprocessWorld
    // function
    SpectatorCamera spectatorCamera;

public:
    GameWorld( const char* name );
    ~GameWorld();

    // Preprocess functionallity when the world is first loaded.
    WorldResult PreprocessWorld() override;

    SpectatorCamera* GetSpectatorCamera()
    {
        return ( &spectatorCamera );
    }
};

#endif // WORLD_HPP
