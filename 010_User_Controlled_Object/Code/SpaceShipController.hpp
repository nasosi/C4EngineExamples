#ifndef ROTATE_CONTROLLER_HPP
#define ROTATE_CONTROLLER_HPP

#include "C4Controller.h"
#include "Input.hpp"

// TODO: CLEANUP
using namespace C4;

enum
{
    kControllerSpaceship = 'cssh'
};

class SpaceshipController : public Controller
{
private:

    uint32 motionFlags = 0;

    SpaceshipController( const SpaceshipController& other );

    Controller* Replicate() const;

    float rollRate  = 0.0f;
    float pitchRate = 0.0f;
    float velocity  = 0.0f;

    Point3D rotationPivotPoint;


public:

    SpaceshipController();
    SpaceshipController( float rotationRate );

    uint32 GetMotionFlags() const
    {
        return motionFlags;
    }

    void SetMotionFlags( uint32 flags )
    {
        motionFlags = flags;
    }

    float GetRollRate() const;
    void  SetRollRate( const float& rate );

    float GetPitchRate() const;
    void  SetPitchRate( const float& rate );

    static bool ValidNode( const Node* node );

    void Pack( Packer& data, uint32 packFlags ) const override;
    void Unpack( Unpacker& data, uint32 unpackFlags ) override;

    void BuildSettingList( List<Setting>* settingList ) const override;
    void CommitSetting( const Setting* setting ) override;

    void PreprocessController() override;

    void MoveController() override;
};

#endif // ROTATE_CONTROLLER_HPP
