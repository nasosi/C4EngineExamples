// MIT License
//
// Copyright (c) 2023-2026 Athanasios Iliopoulos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// This software may contain work that is licensed under different terms. In that
// event, appropriate license files will be included in close folder proximity
// to said work. You need to ensure that your use of this software is compatible
// with the license of any other work that may be part of this software.


#include "RotateController.hpp"
#include "C4Configuration.h"
#include "C4Geometries.h"
#include "C4Node.h"
#include "Game.hpp"

using namespace C4;

// Some constants to help with conversions
constexpr float pi                                = 3.14159265358979323846f;
constexpr float radPerMillisecond_To_DegPerSec    = 1000.0F * 180.0f / pi;
constexpr float degPerSecond_To_radPerMillisecond = 1.0f / radPerMillisecond_To_DegPerSec;

// Fast remainder function to help with angle wrapping.
float FastRemainder( float x, float y )
{
    if ( y == 0.0f )
    {
        return x;
    }

    float n = ( x / y );
    int   i = int( n ); // Truncate toward zero.

    return x - float( i ) * y;
}

// Wrap an angle in radians to the [-pi, pi] range, so that we don't have to worry about the angle value getting too 
// large over time and losing precision.
float WrapPi( float angle )
{
    angle = FastRemainder( angle, Math::two_pi );

    if ( angle > Math::pi )
    {
        angle = Math::pi - FastRemainder( angle + Math::pi, Math::two_pi );
    }
    else if ( angle < -Math::pi )
    {
        angle = -Math::pi + FastRemainder( -Math::pi - angle, Math::two_pi );
    }

    return angle;
}

RotateController::RotateController() : Controller( kControllerRotate )
{
}

RotateController::RotateController( const float& rotationRateRadPerSec ) : Controller( kControllerRotate ), rotationRate( rotationRateRadPerSec )
{
}

float RotateController::GetRotationRate() const
{
    return rotationRate;
}

void RotateController::SetRotationRate( const float& rate )
{
    rotationRate = rate;
}

bool RotateController::ValidNode( const Node* node )
{
    return ( node->GetNodeType() == kNodeGeometry );
}

void RotateController::Pack( Packer& data, uint32 packFlags ) const
{
    Controller::Pack( data, packFlags );

    // Write the rotation rate.
    data << rotationRate;

    // Write the current rotation angle.
    data << currentRotationAngle;

    // Write the original transform.
    data << originalTransform;
}

void RotateController::Unpack( Unpacker& data, uint32 unpackFlags )
{
    Controller::Unpack( data, unpackFlags );

    // Read the rotation rate.
    data >> rotationRate;

    // Read the current rotation angle.
    data >> currentRotationAngle;

    // Read the original transform.
    data >> originalTransform;
}

void RotateController::BuildSettingList( List<Setting>* settingList ) const
{
    float rotationRateDegPerSec = rotationRate * radPerMillisecond_To_DegPerSec;

    // Set the text and value that appears in the editor interface for the rotation rate.
    // We are picking this from the stringTable located in the Import folder
    const StringTable& table = TheGame->GetStringTable();
    settingList->AppendListElement( new TextSetting( 'rrat', table.GetString( StringID( 'ctls', 'rctl', 'rrat' ) ), rotationRateDegPerSec ) );
}

void RotateController::CommitSetting( const Setting* setting )
{
    // Are we setting the rotation rate?
    if ( setting->GetSettingIdentifier() == 'rrat' )
    {
        // Yes, grab the rate value from the setting and convert to rad/msec.
        const char* text = static_cast<const TextSetting*>( setting )->GetText();
        rotationRate     = Data::StringToFloat( text ) * degPerSecond_To_radPerMillisecond;
    }
}

void RotateController::PreprocessController()
{
    Controller::PreprocessController();

    // Cache the original transform of the target node.
    Node* target      = GetTargetNode();
    originalTransform = target->GetNodeTransform();

    // We'll rotate about the object-space center of the target node's bounding box.
    // Store its bounding box
    Box3D boundingBox;
    target->CalculateBoundingBox( &boundingBox );
    rotationPivotPoint = boundingBox.GetCenter();

    // Set the kGeometryDynamic flag for any geometry nodes, so the engine
    // knows we will be moving or deforming it
    const Node* node = target;
    do
    {
        if ( node->GetNodeType() == kNodeGeometry )
        {
            // Node is a geometry, so get its object.
            GeometryObject* object = static_cast<const Geometry*>( node )->GetObject();

            // Set the kGeometryDynamic flag.
            object->SetGeometryFlags( object->GetGeometryFlags() | kGeometryDynamic );
        }

        // Iterate through entire subtree.
        node = target->GetNextTreeNode( node );
    } while ( node );
}

void RotateController::MoveController()
{
    // Calculate the new rotation angle based on how much time has passed.
    currentRotationAngle = currentRotationAngle + rotationRate * TheTimeMgr->GetFloatDeltaTime();

    // Counter windup by making sure the angle is in the [-pi, pi] range.
    currentRotationAngle = WrapPi( currentRotationAngle );

    // Make a 3x3 rotation matrix about the Z axis.
    auto rotator = Matrix3D::MakeRotationZ( currentRotationAngle );

    // Make a  transform (internally represented by a 4x4 matrix that rotates about the center point.
    Transform3D transform( rotator, rotationPivotPoint - rotator * rotationPivotPoint );

    // Apply the rotation transform to the original transform and
    // assign it to the node as its new transform.
    Node* target = this->GetTargetNode();
    target->SetNodeTransform( originalTransform * transform );

    // Invalidate the target node so that the engine knows it needs to update it.
    target->InvalidateNode();
}

RotateController::RotateController( const RotateController& other ) : Controller( kControllerRotate ), rotationRate( other.rotationRate )
{
}

Controller* RotateController::Replicate() const
{
    return ( new RotateController( *this ) );
}
