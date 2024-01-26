#ifndef STARTWINDOW_HPP
#define STARTWINDOW_HPP

#include "C4Widgets.h"

using namespace C4;

class StartWindow : public Window
{
public:
    StartWindow();

    void PreprocessWidget() override;

private:
    WidgetObserver<StartWindow> startButtonObserver;
    WidgetObserver<StartWindow> continueButtonObserver;
    WidgetObserver<StartWindow> QuitButtonObserver;

    Widget*   continueButton;
    ColorRGBA continueButtonOriginalColor;

    void HandleStartButtonClick( Widget* widget, const WidgetEventData* eventData );
    void HandleContinueButtonClick( Widget* widget, const WidgetEventData* eventData );
    void HandleQuitButtonClick( Widget* widget, const WidgetEventData* eventData );
};

#endif // STARTWINDOW_HPP
