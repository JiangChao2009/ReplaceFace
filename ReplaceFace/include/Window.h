#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <string>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Face.h"

#include <string>
#include <vector>


class Window {

protected:

    static cv::CascadeClassifier classifier;

    std::string imageWindow, controlWindow;
    Face *currentFace;

    cv::Mat originalImage, displayedImage;
    std::vector<Face> faces;
    int selected;

    std::string angle, xshift, yshift, width, height;
    cv::Scalar selectedColor, unselectedColor;
    int selectedThickness, unselectedThickness;

public:

    static void setClassifier(std::string cascade_file);


    Window(std::string imageWindow, std::string controlWindow);
    virtual ~Window();

    void setImage(cv::Mat img);
    void setImage(std::string imgfile);
    
    virtual void updateImage(bool reset = false) = 0;
    virtual void lbuttondown(int x, int y, int flags) {};

    Face * getCurrentFace() { return currentFace; }
    cv::Mat getOriginalImage()  { return originalImage; }
    cv::Mat getDisplayedImage() { return displayedImage; }
    std::string getImageWindow()   { return imageWindow; }
    std::string getControlWindow() { return controlWindow; }

    virtual void createControlWindow();
    void updateTrackbars(Face &f);
    
    static void mouseCallback(int event, int x, int y, int flags, void *userdata);

    static void  angleCallback(int position, void *value);
    static void      xCallback(int position, void *value);
    static void      yCallback(int position, void *value);
    static void  widthCallback(int position, void *value);
    static void heightCallback(int position, void *value);

};

#endif
