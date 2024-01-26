#include "SpaceShipController.hpp"
#include "Input.hpp"

#include "C4Node.h"
#include "C4Configuration.h"
#include "C4Geometries.h"
#include "TSMath.h"
#include "World.hpp"

using namespace C4;

Matrix3D RollPitchYawMatrix(float a, float b, float c)
{
    float ca = Cos(a);
    float cb = Cos(b);
    float cc = Cos(c);
    float sa = Sin(a);
    float sb = Sin(b);
    float sc = Sin(c);

    return Matrix3D(
        ca * cb, -cb * sa, sb,
        cc * sa + ca * sb * sc, ca * cc - sa * sb * sc, -cb * sc,
        -ca * cc * sb + sa * sc, cc * sa * sb + ca * sc, cb * cc);
}
SpaceShipController::SpaceShipController():
    Controller( kControllerSpaceShip )
{

}

SpaceShipController::SpaceShipController(const float &rotationRateRadPerSec):
    Controller      ( kControllerSpaceShip ),
    rollRotationRate_   ( rotationRateRadPerSec ),
    pitchRotationRate_(rotationRateRadPerSec)
{

}

const float &SpaceShipController::GetRollRotationRateRadPerSec( ) const
{
    return rollRotationRate_;
}

void SpaceShipController::SetRollRotationRateRadPerSec( const float &rate )
{
    rollRotationRate_ = rate;
}

bool SpaceShipController::ValidNode( const Node *node )
{
    return ( node->GetNodeType() == kNodeGeometry);
}

void SpaceShipController::Pack( Packer &data, uint32 packFlags ) const
{
    Controller::Pack(data, packFlags);
    // TODO: Not all attributes are currently stored.
    data << rollRotationRate_ << currentRollAngle << currentPitchAngle ;
}

void SpaceShipController::Unpack( Unpacker &data, uint32 unpackFlags )
{
    Controller::Unpack(data, unpackFlags);

    data >> rollRotationRate_ >> currentRollAngle >> currentPitchAngle;
}

void SpaceShipController::BuildSettingList(List<Setting> *settingList ) const
{
    // We append the rate to the list of settings with a string ("Rotation rate"),
    // that will be displayed by the world editor
    float rotationRateDegPerSec = rollRotationRate_ * 1000.0F / Math::two_pi;
    settingList->AppendListElement( new TextSetting(
                                        'rrat',
                                        "Rotation rate (deg/sec)",
                                        rotationRateDegPerSec ) );
}

void SpaceShipController::CommitSetting(const Setting *setting)
{
    // Are we setting the rotation rate?
    if (setting->GetSettingIdentifier() == 'rrat')
    {
        // Yes, grab the rate value from the setting and convert to rad/msec.
        const char *text = static_cast<const TextSetting *>(setting)->GetText();
        rollRotationRate_ = Data::StringToFloat( text ) * Math::two_pi * 0.001F;
    }
}

void SpaceShipController::PreprocessController()
{
    Controller::PreprocessController();

    // Get the original transform of the target node.
    Node *target = GetTargetNode( );

    // Set the kGeometryDynamic flag for any geometry nodes, so the engine
    // knows we will be moving or deforming it
    const Node *node = target;
    do
    {
        if ( node->GetNodeType() == kNodeGeometry )
        {
            // Node is a geometry, so get its object.
            GeometryObject *object = static_cast<const Geometry *>( node )->GetObject();

            // Set the kGeometryDynamic flag.
            object->SetGeometryFlags( object->GetGeometryFlags() | kGeometryDynamic );
        }

        // Iterate through entire subtree.
        node = target->GetNextTreeNode( node );
    } while ( node) ;
}

void SpaceShipController::MoveController( )
{
    // We'll rotate about the object-space center of the target node's bounding box.
    Node *target = GetTargetNode( );
    Box3D boundingBox;
    target->CalculateBoundingBox( &boundingBox );

    Point3D objectCenter =  boundingBox.GetCenter();

    auto dt = TheTimeMgr->GetFloatDeltaTime();

    float rotaryAcceleration = 0.000005f;

    auto prevRollAngle = currentRollAngle;
    auto prevPitchAngle = currentPitchAngle;


    bool rotRoll = false;
    if (motionFlags & kRollRight )
    {
        rollRotationRate_ += dt * rotaryAcceleration;
        rollRotationRate_ = Fmin(rollRotationRate_, 0.01f);
        rotRoll = true;
    }

    if (motionFlags & kRollLeft)
    {
        rollRotationRate_ -= dt * rotaryAcceleration;
        rollRotationRate_ = Fmax(rollRotationRate_, -0.01f);

        rotRoll = true;
    }

    if (rotRoll != true)
    {
        rollRotationRate_ += -Fsgn(rollRotationRate_) * dt * rotaryAcceleration;
    }

    currentRollAngle = currentRollAngle + rollRotationRate_ * dt;

    bool rotPitch = false;
    if (motionFlags & kPitchUp)
    {
        pitchRotationRate_ += dt * rotaryAcceleration / 2.0;
        pitchRotationRate_ = Fmin(pitchRotationRate_, 0.01f);
        rotPitch = true;
    }

    if (motionFlags & kPitchDown)
    {
        pitchRotationRate_ -= dt * rotaryAcceleration / 2.0;
        pitchRotationRate_ = Fmax(pitchRotationRate_, -0.01f);

        rotPitch = true;
    }

    if (rotPitch != true)
    {
        pitchRotationRate_ += -Fsgn(pitchRotationRate_) * dt * rotaryAcceleration / 2.0;
    }

    if (motionFlags & kAccelerate)
    {
        velocity += dt * 0.00001;
    }

    if (motionFlags & kDecelerate)
    {
        velocity -= dt * 0.00001;
        velocity = Fmax(0.0, velocity);
    }

    // Calculate the new rotation angle based on how much time has passed.
    currentPitchAngle = currentPitchAngle + pitchRotationRate_ * dt;

    // Counter windup by making sure the angle is in the [-pi, pi] range.

    if (currentRollAngle > Math::pi)
    {
        currentRollAngle -= Math::two_pi;
    }
    else if (currentRollAngle < -Math::pi)
    {
        currentRollAngle += Math::two_pi;
    }

    if (currentPitchAngle > Math::pi)
    {
        currentPitchAngle -= Math::two_pi;
    }
    else if (currentPitchAngle < -Math::pi)
    {
        currentPitchAngle += Math::two_pi;
    }
    
    auto rotator = RollPitchYawMatrix(currentRollAngle - prevRollAngle, 0, currentPitchAngle - prevPitchAngle);

    auto tr = target->GetNodeTransform();

    // Make a 3x4 transform that rotates about the center point.
    Transform4D transform(rotator, objectCenter - rotator * objectCenter + Vector3D(0,0, velocity*dt ) );
    //Transform4D transform2(1,0,0, 0,   0,1,0, 0,    0,0,1, velocity * dt);

    auto trf = tr*transform ;


    // Apply the rotation transform to the original transform and
    // assign it to the node as its new transform.
    target->SetNodeTransform(trf);

    // Invalidate the target node so that it gets updated properly.
    target->InvalidateNode();

    auto world = static_cast<GameWorld*>(TheWorldMgr->GetWorld());
    if (world)
    {
        auto camera = world->GetSpectatorCamera();
        camera->LookAtPoint(objectCenter);
        //camera->SetSpectatorFlags(camera->GetSpectatorFlags() & ~spectatorFlag);
    }

}

SpaceShipController::SpaceShipController( const SpaceShipController &other ):
    Controller( kControllerSpaceShip ),
    rollRotationRate_( other.rollRotationRate_ ),
    pitchRotationRate_(other.pitchRotationRate_)
{

}

Controller *SpaceShipController::Replicate( ) const
{
    return (new SpaceShipController( *this ) );
}
