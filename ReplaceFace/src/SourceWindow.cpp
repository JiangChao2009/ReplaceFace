#include "SourceWindow.h"
#include "DestinationWindow.h"
#include "Face.h"

#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace std;
using namespace cv;


SourceWindow::SourceWindow(string imageWindow, string controlWindow)
    : Window(imageWindow, controlWindow), dw(NULL)
{}


SourceWindow::~SourceWindow() {
    if (currentFace != NULL)
        destroyWindow(controlWindow);
}



void SourceWindow::setDestination(DestinationWindow *destination) {
    dw = destination;
}


void SourceWindow::updateImage(bool reset) {
    originalImage.copyTo(displayedImage);
    if (currentFace == NULL) {
        for (auto face = faces.begin(); face != faces.end(); ++face)
            face->drawEllipse(displayedImage, unselectedColor, unselectedThickness);
    } else {
        currentFace->drawEllipse(displayedImage, selectedColor, selectedThickness);
    }
    imshow(imageWindow, displayedImage);
    if (dw != NULL)
        dw->updateImage(false);
}


void SourceWindow::lbuttondown(int x, int y, int flags) {
    //bool noControlWindow = (currentFace == NULL);
    currentFace = NULL;
    for (auto face = faces.begin(); face != faces.end(); ++face)
        if (face->contains(x, y)) {
            currentFace = &(*face);
            //if (noControlWindow)
            //    createControlWindow();
            updateTrackbars(*face);
            break;
        }
    //if (!noControlWindow && currentFace == NULL)
    //    destroyWindow(controlWindow);
    updateImage(false);
}
