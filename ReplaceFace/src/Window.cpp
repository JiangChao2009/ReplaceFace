#include "Window.h"

#include <iostream>

using namespace cv;
using namespace std;


CascadeClassifier Window::classifier;


void Window::setClassifier(string cascade_file) {
    if (!classifier.load(cascade_file)) {
        cerr << "Unable to load cascade file: '" << cascade_file << "'" << endl;
        exit(-1);
    }
}


Window::Window(string imageWindow, string controlWindow)
    : imageWindow(imageWindow), controlWindow(controlWindow), currentFace(NULL)
{
    selectedColor = Scalar(255, 255, 0);
    selectedThickness = 3;
    unselectedColor = Scalar(255, 255, 255);
    unselectedThickness = 1;
    angle  = "Angle";
    xshift = "x-Shift";
    yshift = "y-Shift";
    width  = "Scale Width";
    height = "Scale Height";

    namedWindow(imageWindow, CV_WINDOW_AUTOSIZE);
    setMouseCallback(imageWindow, Window::mouseCallback, (void *)this);
    createControlWindow();
}


Window::~Window() {
    destroyWindow(imageWindow);
    destroyWindow(controlWindow);
}


void Window::setImage(string imgfile) {
    setImage(imread(imgfile, CV_LOAD_IMAGE_UNCHANGED));
}

void Window::setImage(Mat img) {
    img.copyTo(originalImage);
    Mat gray;
    cvtColor(originalImage, gray, CV_BGR2GRAY);
    equalizeHist(gray, gray);
    vector<Rect> face_rects;
    classifier.detectMultiScale(gray, face_rects, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30));
    faces.clear();
    for (auto fr = face_rects.begin(); fr != face_rects.end(); ++fr)
        faces.push_back(Face(*fr));
    updateImage(true);
}


void Window::createControlWindow() {
    namedWindow(controlWindow, CV_WINDOW_AUTOSIZE);
    createTrackbar(angle,  controlWindow, NULL, MAX_ANGLE - MIN_ANGLE,             Window::angleCallback,  (void*)this);
    createTrackbar(xshift, controlWindow, NULL, MAX_XSHIFT - MIN_XSHIFT,           Window::xCallback,      (void*)this);
    createTrackbar(yshift, controlWindow, NULL, MAX_YSHIFT - MIN_YSHIFT,           Window::yCallback,      (void*)this);
    createTrackbar(width,  controlWindow, NULL, MAX_WIDTHSCALE -  MIN_WIDTHSCALE,  Window::widthCallback,  (void*)this);
    createTrackbar(height, controlWindow, NULL, MAX_HEIGHTSCALE - MIN_HEIGHTSCALE, Window::heightCallback, (void*)this);
}

void Window::updateTrackbars(Face &f) {
    setTrackbarPos(angle,  controlWindow, f.getAngle());
    setTrackbarPos(xshift, controlWindow, f.getX());
    setTrackbarPos(yshift, controlWindow, f.getY());
    setTrackbarPos(width,  controlWindow, f.getWidth());
    setTrackbarPos(height, controlWindow, f.getHeight());
}


void Window::mouseCallback(int event, int x, int y, int flags, void *userdata) {
    Window *window = (Window *)userdata;
    switch (event) {
        case EVENT_MOUSEMOVE: break;
        case EVENT_LBUTTONDOWN: window->lbuttondown(x, y, flags); break;
        case EVENT_RBUTTONDOWN: break;
        case EVENT_MBUTTONDOWN: break;
        case EVENT_LBUTTONUP: break;
        case EVENT_RBUTTONUP: break;
        case EVENT_MBUTTONUP: break;
        case EVENT_LBUTTONDBLCLK: break;
        case EVENT_RBUTTONDBLCLK: break;
        case EVENT_MBUTTONDBLCLK: break;
    }
}


void Window::angleCallback(int position, void *value) {
    Window *w = (Window *)value;
    Face *f = w->getCurrentFace();
    if (f != NULL) {
        f->setAngle(position);
        w->updateImage(false);
    }
}

void Window::xCallback(int position, void *value) {
    Window *w = (Window *)value;
    Face *f = w->getCurrentFace();
    if (f != NULL) {
        f->setX(position);
        w->updateImage(false);
    }
}

void Window::yCallback(int position, void *value) {
    Window *w = (Window *)value;
    Face *f = w->getCurrentFace();
    if (f != NULL) {
        f->setY(position);
        w->updateImage(false);
    }
}

void Window::widthCallback(int position, void *value) {
    Window *w = (Window *)value;
    Face *f = w->getCurrentFace();
    if (f != NULL) {
        f->setWidth(position);
        w->updateImage(false);
    }
}

void Window::heightCallback(int position, void *value) {
    Window *w = (Window *)value;
    Face *f = w->getCurrentFace();
    if (f != NULL) {
        f->setHeight(position);
        w->updateImage(false);
    }
}
