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


#include "BoxesGrid.hpp"

#include <C4World.h>


using namespace C4;


void AddBoxGridToTheWorld( World* world, float width, float spacing, float z_location )
{
    auto boxSize = Vector3D( width, width, width );
    for ( float x = -9; x < 9; x += spacing )
    {
        for ( float y = -9; y < 9; y += spacing )
        {
            for ( float z = -9; z < 9; z += spacing )
            {
                // Create a box of size boxSize and position it
                auto box = new BoxGeometry( boxSize );
                box->SetNodePosition( Point3D( x, y, z + z_location ) );

                // Append a BoxShape subnode to take care of the collision boundaries
                // Note: there is no reason to define a position since it is a subnode
                // of our box and the parent transformation matrix is carried over
                auto boxShape = new BoxShape( boxSize );
                box->AppendSubnode( boxShape );

                // Create a rigid body controller and assign it to our box
                auto rigidBody = new RigidBodyController;
                rigidBody->SetRestitutionCoefficient( 0.6f ); // So we can bounce a bit
                box->SetController( rigidBody );

                // Because the rigid body controller will take care of collisions
                // disable the collision from the object
                box->GetObject()->SetCollisionExclusionMask( kCollisionExcludeAll );

                // Build the primitive (levels and collision data)
                box->GetObject()->BuildPrimitive( box );

                // Add the node to the world
                world->AddNewNode( box );
            }
        }
    }
}
