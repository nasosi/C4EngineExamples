#include "C4World.h"

#include "BoxesGrid.hpp"
#include "C4World.h"

using namespace C4;

void addBoxesGridToTheWorld( World *world, float width, float spacing, float z_location )
{
    auto boxSize = Vector3D( width, width, width ) ;
    for ( float x = -9; x < 9; x+= spacing )
    {
        for ( float y = -9; y < 9; y+= spacing )
        {
            for ( float z = -9; z < 9; z+= spacing )

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
