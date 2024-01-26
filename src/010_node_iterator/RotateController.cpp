#include "RotateController.hpp"
#include "C4Configuration.h"
#include "C4Geometries.h"
#include "C4Node.h"

#include "NodeIterator.hpp"

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

    data << rotationRate_ << currentRotationAngle_ << originalTransform_;
}

void RotateController::Unpack( Unpacker& data, uint32 unpackFlags )
{
    Controller::Unpack( data, unpackFlags );

    data >> rotationRate_ >> currentRotationAngle_ >> originalTransform_;
}

void RotateController::BuildSettingList( List<Setting>* settingList ) const
{
    float rotationRateDegPerSec = rotationRate_ * 1000.0F / Math::two_pi;
    settingList->AppendListElement( new TextSetting( 'rrat', "Rotation rate (deg/sec)", rotationRateDegPerSec ) );
}

void RotateController::CommitSetting( const Setting* setting )
{
    if ( setting->GetSettingIdentifier() == 'rrat' )
    {
        const char* text = static_cast<const TextSetting*>( setting )->GetText();
        rotationRate_    = Data::StringToFloat( text ) * Math::two_pi * 0.001F;
    }
}

void RotateController::PreprocessController()
{
    Controller::PreprocessController();

    Node* target       = GetTargetNode();
    originalTransform_ = target->GetNodeTransform();

    // A list of all geometry nodes
    auto geometryNodesList = IteratableNodeListOfType<Geometry>( target );

    for ( const auto& geometry : geometryNodesList )
    {
        Engine::Report( String<>( "Setting dynamic flag for: " ) + geometry.GetNodeName() );

        GeometryObject* object = geometry.GetObject();
        object->SetGeometryFlags( object->GetGeometryFlags() | kGeometryDynamic );
    }
}

void RotateController::MoveController()
{
    Node* target = GetTargetNode();
    Box3D boundingBox;
    target->CalculateBoundingBox( &boundingBox );

    Point3D objectCenter = boundingBox.GetCenter();

    currentRotationAngle_ = currentRotationAngle_ + rotationRate_ * TheTimeMgr->GetFloatDeltaTime();

    if ( currentRotationAngle_ > Math::pi )
    {
        currentRotationAngle_ -= Math::two_pi;
    }
    else if ( currentRotationAngle_ < -Math::pi )
    {
        currentRotationAngle_ += Math::two_pi;
    }

    auto rotator = Matrix3D::MakeRotationZ( currentRotationAngle_ );

    Transform4D transform( rotator, objectCenter - rotator * objectCenter );

    target->SetNodeTransform( originalTransform_ * transform );

    target->InvalidateNode();
}

RotateController::RotateController( const RotateController& other ) : Controller( kControllerRotate ), rotationRate_( other.rotationRate_ )
{
}

Controller* RotateController::Replicate() const
{
    return ( new RotateController( *this ) );
}
