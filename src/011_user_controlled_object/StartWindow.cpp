#include "C4Engine.h"
#include "C4World.h"

#include "StartWindow.hpp"

//#include <iostream>

StartWindow::StartWindow() :
    Window( APPNAME "/Panel/ImprovedStart" ),

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
        continueButtonOriginalColor = continueButton->GetWidgetColor();
        continueButton->SetWidgetColor( continueButtonOriginalColor * 0.5 );
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
            continueButton->SetWidgetColor( continueButtonOriginalColor );
            continueButton->SetWidgetState( continueButton->GetWidgetState() & ( ~kWidgetDisabled ) );
        }

        if ( TheWorldMgr->GetWorld() == nullptr )
        {
            TheWorldMgr->LoadWorld( APPNAME "/World/011_user_controlled_object" );
        }
        else
        { // Reload the loaded world
            TheWorldMgr->LoadWorld( TheWorldMgr->GetWorld()->GetWorldName() );
        }
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
