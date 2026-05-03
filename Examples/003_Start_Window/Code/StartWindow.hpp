#pragma once


#include <C4Widgets.h>


using namespace C4;


class StartWindow : public Window
{
public:

    StartWindow();

    void PreprocessWidget() override;

private:

    // An observer that will handle clicks on the start button.
    WidgetObserver<StartWindow> startButtonObserver;

    // We will use this function as the callback for the start button observer. When 
    // the start button is clicked, this function will hide the start window and load the world.
    void HandleStartButtonClick( Widget* widget, const WidgetEventData* eventData );
};

