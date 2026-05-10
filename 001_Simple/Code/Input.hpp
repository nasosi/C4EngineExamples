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


// Enums of the action types we want to support
enum : ActionType
{
    kActionForward  = 'frwd',
    kActionBackward = 'bkwd',
    kActionLeft     = 'left',
    kActionRight    = 'rght',
    kActionUp       = 'jump',
    kActionDown     = 'down',
};


// MovementAction is an input binding that modifies spectator camera movement state.
//
// Each instance represents a single movement direction (or behavior) and is
// parameterized by a spectatorFlag. When the action is engaged, that flag is
// added to the camera's spectator state; when disengaged, it is removed.
//
// This allows multiple movement inputs (e.g. left + forward) to be active
// simultaneously using a single bitmask stored in the camera.
//
// For your game to be interesting, you will likely other types of actions.
class MovementAction : public Action
{
private:

    uint32 spectatorFlag;

public:

    MovementAction( ActionType type, uint32 specFlag );
    ~MovementAction() = default;

    // Called when the action type is engaged
    void HandleEngage();

    // Called when the action type is disengaged
    void HandleDisengage();
};