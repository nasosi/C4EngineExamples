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
