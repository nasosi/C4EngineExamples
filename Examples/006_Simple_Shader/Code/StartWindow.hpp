#pragma once


#include <C4Widgets.h>


using namespace C4;


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
    ColorRGBA continueButtonEnabledColor = ColorRGB( 0.5, 0.5, 0.5 );

    void HandleStartButtonClick( Widget* widget, const WidgetEventData* eventData );
    void HandleContinueButtonClick( Widget* widget, const WidgetEventData* eventData );
    void HandleQuitButtonClick( Widget* widget, const WidgetEventData* eventData );
};
