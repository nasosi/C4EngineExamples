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

using namespace C4;

enum
{
    kControllerRotate = 'crot'
};

// Based on the twist controller from the TS wiki modified with comments, style
// changes.
// Note : Remember to enable collisions in the editor geomerty properties if desired
class RotateController : public Controller
{
public:
    RotateController();
    RotateController( const float& rotationRateRadPerSec );

    float GetRotationRate() const;

    void SetRotationRate( const float& rotationRate );

    // Called by the world editor so that it knows if the controller can be used
    // with a specific type of node. Here we will only allow Geometry Nodes.
    static bool ValidNode( const Node* node );

    // Serialization functions. Mostly Used to save the state of the controller
    // to a file if required
    void Pack( Packer& data, uint32 packFlags ) const override;
    void Unpack( Unpacker& data, uint32 unpackFlags ) override;

    // Functions to enable the World Editor to query and modify our controller
    // settings
    void BuildSettingList( List<Setting>* settingList ) const override;
    void CommitSetting( const Setting* setting ) override;

    void PreprocessController() override;

    // Called at every frame to move the target node (for example a geometry node).
    void MoveController() override;

private:
    RotateController( const RotateController& other );

    Controller* Replicate() const override;

    float       rotationRate         = Math::two_pi / 1000.0F; // In radians per millisecond.
    float       currentRotationAngle = 0.0; // The current angle, in radians.
    Transform3D originalTransform; // The target's original transform.
    Point3D     rotationPivotPoint; // The point around which the target will rotate.
};

#endif // ROTATE_CONTROLLER_HPP
