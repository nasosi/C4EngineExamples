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
