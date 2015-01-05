#include "DestinationWindow.h"
#include "SourceWindow.h"

#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


DestinationWindow::DestinationWindow(string imageWindow, string controlWindow)
    : Window(imageWindow, controlWindow), sw(NULL), transplants()
{}

DestinationWindow::~DestinationWindow() {
    if (currentFace != NULL)
        destroyWindow(controlWindow);
}


void DestinationWindow::setSource(SourceWindow *source) {
    sw = source;
}


void DestinationWindow::setMapping() {
    Face * swface = sw->getCurrentFace();
    for (size_t i = 0; i < transplants.size(); ++i)
        if (transplants[i].second == currentFace) {
            if (swface == NULL)
                transplants.erase(transplants.begin() + i);
            else
                transplants[i].first = swface;
            return;
        }
    if (swface != NULL)
        transplants.push_back(pair<Face*,Face*>(swface, currentFace));
}

Face * DestinationWindow::getMapping(Face *dst) {
    for (size_t i = 0; i < transplants.size(); ++i)
        if (dst == transplants[i].second)
            return transplants[i].first;
    return NULL;
}


void DestinationWindow::updateImage(bool reset) {
    if (reset)
        transplants.clear();
    originalImage.copyTo(displayedImage);
    for (auto p = transplants.begin(); p != transplants.end(); ++p)
        p->first->copyToOval(sw->getOriginalImage(), *(p->second), displayedImage);
    //if (currentFace == NULL)
        for (auto face = faces.begin(); face != faces.end(); ++face) {
            if (currentFace == &(*face))
                face->drawEllipse(displayedImage, selectedColor, selectedThickness);
            else
                face->drawEllipse(displayedImage, unselectedColor, unselectedThickness);
        }
    imshow(imageWindow, displayedImage);
}


void DestinationWindow::lbuttondown(int x, int y, int flags) {
    currentFace = NULL;
    for (auto face = faces.begin(); face != faces.end(); ++face)
        if (face->contains(x, y)) {
            currentFace = &(*face);
            setMapping();
            updateTrackbars(*face);
            break;
        }
    updateImage(false);
}


void DestinationWindow::saveImage(string filename) {
    Mat m;
    originalImage.copyTo(m);
    for (auto p = transplants.begin(); p != transplants.end(); ++p)
        p->first->copyToOval(sw->getOriginalImage(), *(p->second), m);
    imwrite(filename, m);
}
