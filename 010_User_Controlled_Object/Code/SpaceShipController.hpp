#ifndef ROTATE_CONTROLLER_HPP
#define ROTATE_CONTROLLER_HPP

#include "C4Controller.h"
#include "Input.hpp"

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
    SpaceShipController( float rotationRate );

    float GetRollRate() const;
    void  SetRollRate( const float& rate );

    float GetPitchRate() const;
    void  SetPitchRate( const float& rate );

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

    void PreprocessController() override;

    // Called at every frame to move the target node (for example a geometry node).
    void MoveController() override;

private:
    template <Type type, uint32 Flag>
    friend class SpaceShipControlAction; // So it can access motionFlags

    uint32 motionFlags = 0;

    SpaceShipController( const SpaceShipController& other );

    // Create a new controller based on the state of "this".
    Controller* Replicate() const;

    float rollRate  = 0.0f;
    float pitchRate = 0.0f;
    float velocity  = 0.0f;

    Point3D rotationPivotPoint;
};

#endif // ROTATE_CONTROLLER_HPP
