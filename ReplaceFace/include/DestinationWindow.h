#ifndef DESTINATIONWINDOW_H
#define DESTINATIONWINDOW_H

#include "Window.h"
#include "Face.h"

#include <vector>
#include <utility>

class SourceWindow;



class DestinationWindow : public Window {

    SourceWindow *sw;
    std::vector<std::pair<Face*,Face*> > transplants;

public:

    DestinationWindow(std::string imageWindow, std::string controlWindow);
    virtual ~DestinationWindow();

    void setSource(SourceWindow *source);
    virtual void updateImage(bool reset);
    virtual void lbuttondown(int x, int y, int flags);
    void saveImage(std::string filename);

private:

    void setMapping();
    Face * getMapping(Face *dst);

};

#endif
