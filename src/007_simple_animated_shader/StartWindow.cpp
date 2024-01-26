#include "C4World.h"
#include "C4Engine.h"

#include "StartWindow.hpp"

StartWindow::StartWindow( ) :
    Window(APPNAME "/Panel/ImprovedStart" ),

    startButtonObserver     ( this, &StartWindow::HandleStartButtonClick ),
    continueButtonObserver  ( this, &StartWindow::HandleContinueButtonClick ),
    quitButtonObserver      ( this, &StartWindow::HandleQuitButtonClick )
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
        continueButtonOriginalColor = continueButton->GetWidgetColor( );
        continueButton->SetWidgetColor( continueButtonOriginalColor * 0.5 );
    }


    button = FindWidget( "QuitButton" );
    if ( button )
    {
        button->SetObserver( &quitButtonObserver );
    }
}

void StartWindow::HandleStartButtonClick( Widget *, const WidgetEventData *eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        if( continueButton )
        {
            continueButton->SetWidgetColor( continueButtonOriginalColor );
            continueButton->SetWidgetState( continueButton->GetWidgetState() & (~kWidgetDisabled) );
        }

        if ( TheWorldMgr->GetWorld() == nullptr )
        {
            TheWorldMgr->LoadWorld( APPNAME "/World/simple_shader_animated" );

        }
        else
        {   // Reload the loaded world
            auto c = (const char *) TheWorldMgr->GetWorld()->GetWorldName( );

            TheWorldMgr->UnloadWorld();

            TheWorldMgr->LoadWorld( c );

        }
        this->HideWidget();
    }
}

void StartWindow::HandleContinueButtonClick( Widget *, const WidgetEventData *eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        this->HideWidget();
    }
}

void StartWindow::HandleQuitButtonClick( Widget *, const WidgetEventData *eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        TheEngine->Quit();
    }
}
