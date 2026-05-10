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


#pragma once

#include "C4Input.h"

using namespace C4;

enum : ActionType
{
    kActionForward  = 'frwd',
    kActionBackward = 'bkwd',
    kActionLeft     = 'left',
    kActionRight    = 'rght',
    kActionUp       = 'jump',
    kActionDown     = 'down',
};

// Bitflags for the different motion states of the spaceship. Bitmasks are used so
// that we can combine multiple states at the same time (for example, accelerating
// while pitching up and rolling left). The actual values of the flags don't matter.
enum
{
    kPitchUp    = 1 << 0,
    kPitchDown  = 1 << 1,
    kRollLeft   = 1 << 2,
    kRollRight  = 1 << 3,
    kYawLeft    = 1 << 4,
    kYawRight   = 1 << 5,
    kAccelerate = 1 << 6,
    kDecelerate = 1 << 7
};

// We don't plan to dynamically update the type and flag parameters, so we can use a
// template to bind them at compile time.
template <ActionType type, uint32 Flag>
class SpaceshipControlAction : public Action
{
public:

    SpaceshipControlAction() : Action( type )
    {
    }

    ~SpaceshipControlAction()
    {
    }

    void HandleEngage()
    {
        GameWorld* world = static_cast<GameWorld*>( TheWorldMgr->GetWorld() );
        if ( world )
        {
            SpaceshipController* controller = world->GetSpaceshipController();
            if ( controller )
            {
                controller->SetMotionFlags( controller->GetMotionFlags() | Flag );
            }
        }
    }

    void HandleDisengage()
    {
        GameWorld* world = static_cast<GameWorld*>( TheWorldMgr->GetWorld() );
        if ( world )
        {
            SpaceshipController* controller = world->GetSpaceshipController();
            if ( controller )
            {
                controller->SetMotionFlags( controller->GetMotionFlags() & ~Flag );
            }
        }
    }
};
