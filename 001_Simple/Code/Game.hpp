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

#include <C4Application.h>
#include <C4Markers.h>
#include <C4StringTable.h>


using namespace C4;


extern "C"
{
    // Entry point, C4Engine Engine will call this function to create an instance of your game
    C4_MODULE_EXPORT C4::Application* CreateApplication();
}


// Our game world will include a "Locator" so the code know where to place the camera when the game starts.
// Here we define its type to be a spectator.
enum : LocatorType
{
    kLocatorSpectator = 'spec'
};

class Game : public Application
{
private:

    // The string table includes various strings for our game. Check Import\Examples\001_Simple\Game\gameStrings.txt
    // For the simple example, we only need a string to indicate the "Locator" and "Spectator" strings to be used
    // in the C4 Engine interface. You can find them in the Object Tab under "Marker".
    StringTable stringTable;

    // We use this to register our specator locator with the engine
    LocatorRegistration spectatorLocatorRegistrar;

    // The various movement actions we support
    MovementAction forwardAction;
    MovementAction backwardAction;
    MovementAction leftAction;
    MovementAction rightAction;
    MovementAction upAction;
    MovementAction downAction;

public:
    Game();

    // The destructor will be used to clean up any resources we created, or other items
    // the engine requires.
    ~Game();

    // Function to make the World Manager create an instance of our custom world class 
    // ( GameWorld ) instead of the default, World class
    static World* CreateWorld( const char* name, void* cookie );
};
