#include "C4World.h"

#include "StartWindow.hpp"

StartWindow::StartWindow() : Window( APPNAME "/Panel/SimpleStart" ), startButtonObserver( this, &StartWindow::HandleStartButtonClick )
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
}

void StartWindow::HandleStartButtonClick( Widget*, const WidgetEventData* eventData )
{
    if ( eventData->eventType == kEventWidgetActivate )
    {
        this->HideWidget();

        TheWorldMgr->LoadWorld( APPNAME "/World/simple" );
    }
}
