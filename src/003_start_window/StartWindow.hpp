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

    void HandleStartButtonClick( Widget* widget, const WidgetEventData* eventData );
};

#endif // STARTWINDOW_HPP
