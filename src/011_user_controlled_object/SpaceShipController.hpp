#ifndef ROTATE_CONTROLLER_HPP
#define ROTATE_CONTROLLER_HPP

#include "C4Controller.h"

// TODO: CLEANUP
using namespace C4;

enum
{
    kControllerSpaceShip = 'cssh'
};

class SpaceShipController : public Controller
{
public:
    SpaceShipController();
    SpaceShipController( const float& rotationRateRadPerSec );

    const float& GetRollRotationRateRadPerSec() const;

    void SetRollRotationRateRadPerSec( const float& rotationRate );

    // Called by the world editor so that it knows if the controller can be used
    // with a specific type of node. Here we will only allow Geometry Nodes.
    static bool ValidNode( const Node* node );

    // Serialization functions. Mostly Used to save the state of the controller
    // to a file if required
    void Pack( Packer& data, uint32 packFlags ) const override;
    void Unpack( Unpacker& data, uint32 unpackFlags ) override;

    // Functions to enable the World Editor to query and modify our controller
    // settings
    void BuildSettingList( List<Setting>* settingList ) const override;
    void CommitSetting( const Setting* setting ) override;

    void PreprocessController();

    // Called at every frame to move the target node (for example a geometry node).
    void MoveController();

public:
    uint32 motionFlags = 0;

private:
    SpaceShipController( const SpaceShipController& other );

    // Create a new controller based on the state of "this".
    Controller* Replicate() const;

    float rollRotationRate_  = 0; // In radians per millisecond.
    float pitchRotationRate_ = 0; // In radians per millisecond.
    float currentRollAngle   = 0.0; // The current angle, in radians.
    float currentPitchAngle  = 0.0; // The current angle, in radians.
    float velocity           = 0.0;
};

#endif // ROTATE_CONTROLLER_HPP
