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


#include "StartWindow.hpp"
#include "AppInfo.hpp"

#include <C4World.h>

// The panel of the window is defined in the file "Panel/SimpleStart.pan". We used
// the Panel commands from the C4 start button to design it.
StartWindow::StartWindow() : Window( APP_PATH "/Panel/SimpleStart" ), startButtonObserver( this, &StartWindow::HandleStartButtonClick )
{
}

void StartWindow::PreprocessWidget()
{

    Window::PreprocessWidget();

    // The observer will handle clicks on the start button. When the start button is 
    // clicked, the observer will call the function HandleStartButtonClick, which will
    // hide the start window and load the world. "StartButton" is the 'Widget 
    // Identifier'  property defined in the C4 Engine Panel Editor.
    auto button = FindWidget( "StartButton" );
    if ( button )
    {
        button->SetObserver( &startButtonObserver );
    }
}

void StartWindow::HandleStartButtonClick( Widget*, const WidgetEventData* eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        this->HideWidget();

        TheWorldMgr->LoadWorld( APP_PATH "/World/simple" );
    }
}
