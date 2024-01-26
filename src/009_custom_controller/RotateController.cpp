#include "RotateController.hpp"
#include "C4Configuration.h"
#include "C4Geometries.h"
#include "C4Node.h"

using namespace C4;

RotateController::RotateController() : Controller( kControllerRotate )
{
}

RotateController::RotateController( const float& rotationRateRadPerSec ) : Controller( kControllerRotate ), rotationRate_( rotationRateRadPerSec )
{
}

const float& RotateController::GetRotationRateRadPerSec() const
{
    return rotationRate_;
}

void RotateController::SetRotationRateRadPerSec( const float& rate )
{
    rotationRate_ = rate;
}

bool RotateController::ValidNode( const Node* node )
{
    return ( node->GetNodeType() == kNodeGeometry );
}

void RotateController::Pack( Packer& data, uint32 packFlags ) const
{
    Controller::Pack( data, packFlags );

    // Write the rotation rate.
    data << rotationRate_;

    // Write the current rotation angle.
    data << currentRotationAngle_;

    // Write the original transform.
    data << originalTransform_;
}

void RotateController::Unpack( Unpacker& data, uint32 unpackFlags )
{
    Controller::Unpack( data, unpackFlags );

    // Read the rotation rate.
    data >> rotationRate_;

    // Read the current rotation angle.
    data >> currentRotationAngle_;

    // Read the original transform.
    data >> originalTransform_;
}

void RotateController::BuildSettingList( List<Setting>* settingList ) const
{
    // We append the rate to the list of settings with a string ("Rotation rate"),
    // that will be displayed by the world editor
    float rotationRateDegPerSec = rotationRate_ * 1000.0F / Math::two_pi;
    settingList->AppendListElement( new TextSetting( 'rrat', "Rotation rate (deg/sec)", rotationRateDegPerSec ) );
}

void RotateController::CommitSetting( const Setting* setting )
{
    // Are we setting the rotation rate?
    if ( setting->GetSettingIdentifier() == 'rrat' )
    {
        // Yes, grab the rate value from the setting and convert to rad/msec.
        const char* text = static_cast<const TextSetting*>( setting )->GetText();
        rotationRate_    = Data::StringToFloat( text ) * Math::two_pi * 0.001F;
    }
}

void RotateController::PreprocessController()
{
    Controller::PreprocessController();

    // Get the original transform of the target node.
    Node* target       = GetTargetNode();
    originalTransform_ = target->GetNodeTransform();

    // Set the kGeometryDynamic flag for any geometry nodes, so the engine
    // knows we will be moving or deforming it
    const Node* node = target;
    do
    {
        if ( node->GetNodeType() == kNodeGeometry )
        {
            // Node is a geometry, so get its object.
            GeometryObject* object = static_cast<const Geometry*>( node )->GetObject();

            Engine::Report( String<>( "Setting dynamic flag for: " ) + node->GetNodeName() );
            // Set the kGeometryDynamic flag.
            object->SetGeometryFlags( object->GetGeometryFlags() | kGeometryDynamic );
        }

        // Iterate through entire subtree.
        node = target->GetNextTreeNode( node );
    } while ( node );
}

void RotateController::MoveController()
{
    // We'll rotate about the object-space center of the target node's bounding box.
    Node* target = GetTargetNode();
    Box3D boundingBox;
    target->CalculateBoundingBox( &boundingBox );

    Point3D objectCenter = boundingBox.GetCenter();

    // Calculate the new rotation angle based on how much time has passed.
    currentRotationAngle_ = currentRotationAngle_ + rotationRate_ * TheTimeMgr->GetFloatDeltaTime();

    // Counter windup by making sure the angle is in the [-pi, pi] range.

    if ( currentRotationAngle_ > Math::pi )
    {
        currentRotationAngle_ -= Math::two_pi;
    }
    else if ( currentRotationAngle_ < -Math::pi )
    {
        currentRotationAngle_ += Math::two_pi;
    }

    // Make a 3x3 rotation matrix about the Z axis.
    auto rotator = Matrix3D::MakeRotationZ( currentRotationAngle_ );

    // Make a 3x4 transform that rotates about the center point.
    Transform4D transform( rotator, objectCenter - rotator * objectCenter );

    // Apply the rotation transform to the original transform and
    // assign it to the node as its new transform.
    target->SetNodeTransform( originalTransform_ * transform );

    // Invalidate the target node so that it gets updated properly.
    target->InvalidateNode();
}

RotateController::RotateController( const RotateController& other ) : Controller( kControllerRotate ), rotationRate_( other.rotationRate_ )
{
}

Controller* RotateController::Replicate() const
{
    return ( new RotateController( *this ) );
}
