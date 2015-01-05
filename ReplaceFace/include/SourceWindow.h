#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include "Window.h"
#include "Face.h"

class DestinationWindow;



class SourceWindow : public Window {

    DestinationWindow *dw;

public:

    SourceWindow(std::string imageWindow, std::string controlWindow);
    virtual ~SourceWindow();

    void setDestination(DestinationWindow *destination);
    virtual void updateImage(bool reset);
    virtual void lbuttondown(int x, int y, int flags);

};

#endif
