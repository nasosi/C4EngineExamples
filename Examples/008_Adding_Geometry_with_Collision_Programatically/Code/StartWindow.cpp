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

    auto button = FindWidget( "StartButton" );
    if ( button )
    {
        button->SetObserver( &startButtonObserver );
    }

    continueButton = FindWidget( "ContinueButton" );
    if ( continueButton )
    {
        continueButton->SetObserver( &continueButtonObserver );
        continueButtonEnabledColor = continueButton->GetWidgetColor();
        continueButton->SetWidgetColor( continueButtonEnabledColor * 0.5 );
    }

    button = FindWidget( "QuitButton" );
    if ( button )
    {
        button->SetObserver( &quitButtonObserver );
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

        TheWorldMgr->LoadWorld( APP_PATH "/World/simple_physics" );
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
