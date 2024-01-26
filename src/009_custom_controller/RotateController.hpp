#ifndef ROTATE_CONTROLLER_HPP
#define ROTATE_CONTROLLER_HPP

#include "C4Controller.h"

using namespace C4;

enum
{
    kControllerRotate = 'crot'
};

// Based on the twist controller from the TS wiki modified with comments, style
// changes.
// Note : Remember to enable collisions in the editor geomerty properties if desired
class RotateController : public Controller
{
public:
    RotateController();
    RotateController( const float& rotationRateRadPerSec );

    const float& GetRotationRateRadPerSec() const;

    void SetRotationRateRadPerSec( const float& rotationRate );

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

private:
    RotateController( const RotateController& other );

    // Create a new controller based on the state of "this".
    Controller* Replicate() const;

    float       rotationRate_         = Math::two_pi / 1000.0F; // In radians per millisecond.
    float       currentRotationAngle_ = 0.0; // The current angle, in radians.
    Transform4D originalTransform_; // The target's original transform.
};

#endif // ROTATE_CONTROLLER_HPP
