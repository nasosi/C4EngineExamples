#pragma once


#include <C4Widgets.h>


using namespace C4;


// Start menu window with options to start a new game, continue, or quit.
// Uses WidgetObservers to handle UI button clicks.
// Each observer forwards events to a handler in this class:
// Start, Continue, and Quit actions are handled separately.
class StartWindow : public Window
{
public:

    StartWindow();

    void PreprocessWidget() override;

private:

    WidgetObserver<StartWindow> startButtonObserver;
    WidgetObserver<StartWindow> continueButtonObserver;
    WidgetObserver<StartWindow> quitButtonObserver;

    Widget*   continueButton             = nullptr;
    
    // We will store the continue button enabled color so we can restore it when the 
    // button is enabled again. Here we just initialize it to a default value, but it 
    // will be set to the actual button color in PreprocessWidget().
    ColorRGBA continueButtonEnabledColor = ColorRGB( 0.5, 0.5, 0.5 );

    void HandleStartButtonClick( Widget* widget, const WidgetEventData* eventData );
    void HandleContinueButtonClick( Widget* widget, const WidgetEventData* eventData );
    void HandleQuitButtonClick( Widget* widget, const WidgetEventData* eventData );
};
