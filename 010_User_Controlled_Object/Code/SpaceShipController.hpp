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
