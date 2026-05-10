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


#include <C4Cameras.h>
#include <C4World.h>


using namespace C4;


class GameWorld : public World
{
private:

    // This is our spectator camera, which we will use as the active camera for the world.
    SpectatorCamera spectatorCamera;
    void            SetupSpectatorCamera();

public:

    // The World constructor takes the file name of the world resource to load.
    GameWorld( const char* name );

    // The destructor is a virtual function, but we don't need to do anything special
    // in it, so we can use the default implementation.
    ~GameWorld() = default;

    // Preprocess functionallity when the world is loaded.
    WorldResult PreprocessWorld() override;

    SpectatorCamera* GetSpectatorCamera()
    {
        return &spectatorCamera;
    }
};