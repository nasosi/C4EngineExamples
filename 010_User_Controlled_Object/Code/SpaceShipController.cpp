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


#include "SpaceshipController.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "World.hpp"

#include "C4Configuration.h"
#include "C4Geometries.h"
#include "C4Node.h"
#include "TSMath.h"

using namespace C4;

constexpr float pi                                = 3.14159265358979323846f;
constexpr float radPerMillisecond_To_DegPerSec    = 1000.0F * 180.0f / pi;
constexpr float degPerSecond_To_radPerMillisecond = 1.0f / radPerMillisecond_To_DegPerSec;

// Some simulation constants
constexpr float maxRollRate        = 0.01f;
constexpr float maxPitchRate       = maxRollRate;
constexpr float linearAcceleration = 0.00001f;
constexpr float rotaryAcceleration = 0.000005f;


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


Matrix3D RollPitchYawMatrix( float a, float b, float c )
{
    float ca = Cos( a );
    float cb = Cos( b );
    float cc = Cos( c );
    float sa = Sin( a );
    float sb = Sin( b );
    float sc = Sin( c );

    return Matrix3D( ca * cb,
                     -cb * sa,
                     sb,
                     cc * sa + ca * sb * sc,
                     ca * cc - sa * sb * sc,
                     -cb * sc,
                     -ca * cc * sb + sa * sc,
                     cc * sa * sb + ca * sc,
                     cb * cc );
}


SpaceshipController::SpaceshipController() : Controller( kControllerSpaceship )
{
}


SpaceshipController::SpaceshipController( float rotationRate ) :
    Controller( kControllerSpaceship ),
    rollRate( rotationRate ),
    pitchRate( rotationRate )
{
}


float SpaceshipController::GetRollRate() const
{
    return rollRate;
}


void SpaceshipController::SetRollRate( const float& rate )
{
    rollRate = rate;
}


float SpaceshipController::GetPitchRate() const
{
    return pitchRate;
}


void SpaceshipController::SetPitchRate( const float& rate )
{
    pitchRate = rate;
}


bool SpaceshipController::ValidNode( const Node* node )
{
    return ( node->GetNodeType() == kNodeGeometry );
}


void SpaceshipController::Pack( Packer& data, uint32 packFlags ) const
{
    Controller::Pack( data, packFlags );

    data << rollRate << pitchRate << velocity;
}


void SpaceshipController::Unpack( Unpacker& data, uint32 unpackFlags )
{
    Controller::Unpack( data, unpackFlags );

    data >> rollRate >> pitchRate >> velocity;
}


void SpaceshipController::BuildSettingList( List<Setting>* settingList ) const
{
    float              rotationRateDegPerSec = rollRate * radPerMillisecond_To_DegPerSec;
    const StringTable& table                 = TheGame->GetStringTable();

    // We pick the string for the rotation rate from the string table, and use the 'rrat' identifier for the setting
    // that will appear in the editor interface. We also set its value from what is stored in the SpaceshipController
    settingList->AppendListElement(
        new TextSetting( 'rrat', table.GetString( StringID( 'ctls', 'ssct', 'rrat' ) ), rotationRateDegPerSec ) );
}


void SpaceshipController::CommitSetting( const Setting* setting )
{
    // Are we setting the rotation rate?
    if ( setting->GetSettingIdentifier() == 'rrat' )
    { // Yes, grab the rate value from the setting and convert to rad/msec.
        const char* text = static_cast<const TextSetting*>( setting )->GetText();
        rollRate         = Data::StringToFloat( text ) * degPerSecond_To_radPerMillisecond;
        pitchRate        = rollRate;
    }
}


void SpaceshipController::PreprocessController()
{
    // Perform any setup required before the controller can be used.
    Controller::PreprocessController();

    // Get the original transform of the target node.
    Node* target = GetTargetNode();

    // Get the object center so we can rotate about it. We'll use the center of the
    // bounding box, which is good enough for our purposes.
    Box3D boundingBox;
    target->CalculateBoundingBox( &boundingBox );
    rotationPivotPoint = boundingBox.GetCenter();

    // Set the kGeometryDynamic flag for any geometry sub nodes, so the engine knows we
    // will be moving it
    const Node* node = target;
    do
    {
        if ( node->GetNodeType() == kNodeGeometry )
        {
            GeometryObject* object = static_cast<const Geometry*>( node )->GetObject();
            object->SetGeometryFlags( object->GetGeometryFlags() | kGeometryDynamic );
        }

        node = target->GetNextTreeNode( node );
    } while ( node );
}


void SpaceshipController::MoveController()
{
    auto dt = TheTimeMgr->GetFloatDeltaTime();

    // Depending on the motion flags (that correspond to user input), calculate
    // rotation rates and velocity
    bool rotRoll = false;
    if ( motionFlags & kRollRight )
    {
        rollRate += dt * rotaryAcceleration;
        rollRate = Fmin( rollRate, maxRollRate );
        rotRoll  = true;
    }

    if ( motionFlags & kRollLeft )
    {
        rollRate -= dt * rotaryAcceleration;
        rollRate = Fmax( rollRate, -maxRollRate );

        rotRoll = true;
    }

    if ( !rotRoll )
    {
        rollRate += -Fsgn( rollRate ) * dt * rotaryAcceleration;
    }

    bool rotPitch = false;
    if ( motionFlags & kPitchUp )
    {
        pitchRate += dt * rotaryAcceleration / 2.0;
        pitchRate = Fmin( pitchRate, maxPitchRate );
        rotPitch  = true;
    }

    if ( motionFlags & kPitchDown )
    {
        pitchRate -= dt * rotaryAcceleration / 2.0;
        pitchRate = Fmax( pitchRate, -maxPitchRate );

        rotPitch = true;
    }

    if ( rotPitch != true )
    {
        pitchRate += -Fsgn( pitchRate ) * dt * rotaryAcceleration / 2.0;
    }

    if ( motionFlags & kAccelerate )
    {
        velocity += dt * linearAcceleration;
    }

    if ( motionFlags & kDecelerate )
    {
        velocity -= dt * linearAcceleration;
        velocity = Fmax( 0.0, velocity );
    }

    // Build delta transform (rotation + translation about pivot) to move the spaceship
    Matrix3D deltaR         = RollPitchYawMatrix( rollRate * dt, 0, pitchRate * dt );
    Vector3D fwdTranslation = deltaR * Vector3D( 0, 0, velocity * dt ); // translation due to thrust in the model's local forward direction
    Vector3D pivotOffset    = rotationPivotPoint - deltaR * rotationPivotPoint; // rotate pivot back to rotation point, then restore it
    Vector3D deltaTranslation = pivotOffset + fwdTranslation;
    Transform3D delta( deltaR, deltaTranslation ); // full incremental motion

    Node* target = GetTargetNode();
    target->SetNodeTransform( target->GetNodeTransform() * delta );
    target->InvalidateNode();
}


SpaceshipController::SpaceshipController( const SpaceshipController& other ) :
    Controller( kControllerSpaceship ),
    rollRate( other.rollRate ),
    pitchRate( other.pitchRate ),
    velocity( other.velocity )
{
}


Controller* SpaceshipController::Replicate() const
{
    return ( new SpaceshipController( *this ) );
}
