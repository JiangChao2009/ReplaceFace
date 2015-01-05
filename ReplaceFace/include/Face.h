#ifndef FACE_H
#define FACE_H

#include "opencv2/objdetect/objdetect.hpp"


const int MIN_ANGLE       = -180,    MAX_ANGLE       = 180,
          MIN_XSHIFT      =  -20,    MAX_XSHIFT      =  20,
          MIN_YSHIFT      =  -20,    MAX_YSHIFT      =  20,
          MIN_WIDTHSCALE  =    1,    MAX_WIDTHSCALE  = 300,
          MIN_HEIGHTSCALE =    1,    MAX_HEIGHTSCALE = 300;



class Face {

    cv::Rect rect;
    cv::RotatedRect rrect;

    int xShift, yShift;
    cv::Point2f center;

public:

    Face(cv::Rect rect);

    void drawEllipse(cv::Mat &image, cv::Scalar color, int thickness = 1);
    bool contains(int x, int y);
    cv::RotatedRect getRotatedRect();
    cv::Mat getFace(const cv::Mat &image);
    void copyToOval(cv::Mat sourceImage, Face destFace, cv::Mat destImage);
    //void copyToOval(cv::Mat source, cv::Mat destination);

    int getAngle();
    int getWidth();
    int getHeight();
    int getX();
    int getY();

    void setAngle(int a);
    void setWidth(int w);
    void setHeight(int h);
    void setX(int x);
    void setY(int y);

private:

    void updateCenter();

};

#endif
