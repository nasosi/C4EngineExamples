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


#include "Input.hpp"
#include "SpaceshipController.hpp"
#include "StartWindow.hpp"


#include "C4Markers.h"
#include "C4StringTable.h"
#include <C4Application.h>


using namespace C4;


extern "C"
{
    C4_MODULE_EXPORT C4::Application* CreateApplication( void );
}


enum : LocatorType
{
    kLocatorSpectator = 'spec'
};


class Game : public Application, Global<Game>
{
private:

    StringTable stringTable;

    LocatorRegistration spectatorLocatorRegistrar;

    // We create a registar so we can register the spaceship controller with the engine
    ControllerReg<SpaceshipController> rotateControllerReg;

    // Here we define the different input actions for controlling the spaceship, using
    // our template-based SpaceshipControlAction to bind the input type and motion flag
    // at compile time.
    SpaceshipControlAction<kActionLeft, kRollLeft>      rollLeft;
    SpaceshipControlAction<kActionRight, kRollRight>    rollRight;
    SpaceshipControlAction<kActionForward, kPitchUp>    pitchUp;
    SpaceshipControlAction<kActionBackward, kPitchDown> pitchDown;
    SpaceshipControlAction<kActionUp, kAccelerate>      accelerate;
    SpaceshipControlAction<kActionDown, kDecelerate>    decelerate;

    StartWindow startWin;

    InputMgr::KeyCallback* previousEscapeCallback;
    void*                  previousEscapeCookie;

    static void HandleEscape( void* cookie );

public:

    Game();
    ~Game();

    static World* CreateWorld( const char* name, void* cookie );

    const StringTable& GetStringTable() const
    {
        return stringTable;
    }
};


extern Game* TheGame;
