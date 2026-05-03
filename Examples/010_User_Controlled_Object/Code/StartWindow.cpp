#include "StartWindow.hpp"
#include "AppInfo.hpp"

#include "C4Engine.h"
#include "C4World.h"

//#include <iostream>

StartWindow::StartWindow() :
    Window( APP_PATH "/Panel/ImprovedStart" ),

    startButtonObserver( this, &StartWindow::HandleStartButtonClick ),
    continueButtonObserver( this, &StartWindow::HandleContinueButtonClick ),
    QuitButtonObserver( this, &StartWindow::HandleQuitButtonClick )
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
        Button->SetObserver( &QuitButtonObserver );
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

        TheWorldMgr->LoadWorld( APP_PATH "/World/011_user_controlled_object" );
    }
}

void StartWindow::HandleContinueButtonClick( Widget* /*widget*/, const WidgetEventData* eventData )
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
