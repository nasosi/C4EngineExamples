#include "StartWindow.hpp"
#include "AppInfo.hpp"

#include <C4Engine.h>
#include <C4World.h>


StartWindow::StartWindow() :

    Window( APP_PATH "/Panel/ImprovedStart" ),

    startButtonObserver( this, &StartWindow::HandleStartButtonClick ),
    continueButtonObserver( this, &StartWindow::HandleContinueButtonClick ),
    quitButtonObserver( this, &StartWindow::HandleQuitButtonClick )
{
}


void StartWindow::PreprocessWidget()
{
    Window::PreprocessWidget();

    auto Button = FindWidget( "StartButton" );
    if ( Button )
    {
        Button->SetObserver( &startButtonObserver );
    }

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
    if ( eventData->eventType == kEventWidgetActivate )
    {
        this->HideWidget();

        if ( continueButton )
        {
            continueButton->SetWidgetColor( continueButtonEnabledColor );
            continueButton->SetWidgetState( continueButton->GetWidgetState() & ( ~kWidgetDisabled ) );
        }

        // We make sure we load the simple_shader_animated world here. The shader is
        // implemented in the world and not in code for the present example.
        TheWorldMgr->LoadWorld( APP_PATH "/World/simple_shader_animated" );
    }
}


void StartWindow::HandleContinueButtonClick( Widget*, const WidgetEventData* eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        this->HideWidget();
    }
}


void StartWindow::HandleQuitButtonClick( Widget*, const WidgetEventData* eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        TheEngine->Quit();
    }
}
