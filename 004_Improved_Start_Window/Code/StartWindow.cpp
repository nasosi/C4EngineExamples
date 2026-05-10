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

#include <C4Engine.h>
#include <C4World.h>


// The window is loaded from a UI panel resource and sets up observers that
// will handle button click events.
StartWindow::StartWindow() :

    Window( APP_PATH "/Panel/ImprovedStart" ),

    // The observers are responsible for calling the appropriate handler functions
    // (like for example HandleStartButtonClick) when the buttons are clicked.
    startButtonObserver( this, &StartWindow::HandleStartButtonClick ),
    continueButtonObserver( this, &StartWindow::HandleContinueButtonClick ),
    quitButtonObserver( this, &StartWindow::HandleQuitButtonClick )
{
}


void StartWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    // Widgets are looked up by their 'Widget Identifier' that can be found in the Panel editor.
    auto Button = FindWidget( "StartButton" );
    if ( Button )
    {
        Button->SetObserver( &startButtonObserver );
    }

    // The Continue button is initially disabled until a new game is started.
    continueButton = FindWidget( "ContinueButton" );
    if ( continueButton )
    {
        continueButton->SetObserver( &continueButtonObserver );
        continueButtonEnabledColor = continueButton->GetWidgetColor();
        continueButton->SetWidgetColor( continueButtonEnabledColor * 0.5 );
    }

    Button = FindWidget( "QuitButton" );
    if ( Button )
    {
        Button->SetObserver( &quitButtonObserver );
    }
}


void StartWindow::HandleStartButtonClick( Widget* /*widget*/, const WidgetEventData* eventData )
{
    // Hides the menu, enables continue state, and loads the first game world.
    if ( eventData->eventType == kEventWidgetActivate )
    {
        this->HideWidget();

        if ( continueButton )
        {
            continueButton->SetWidgetColor( continueButtonEnabledColor );
            continueButton->SetWidgetState( continueButton->GetWidgetState() & ( ~kWidgetDisabled ) );
        }

        TheWorldMgr->LoadWorld( APP_PATH "/World/simple" );
    }
}


// Closes the menu. Game state loading can be added here.
void StartWindow::HandleContinueButtonClick( Widget*, const WidgetEventData* eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        this->HideWidget();
    }
}


// Handler function to quit the application.
void StartWindow::HandleQuitButtonClick( Widget*, const WidgetEventData* eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        TheEngine->Quit();
    }
}
