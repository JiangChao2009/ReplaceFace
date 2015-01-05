#include "Face.h"

using namespace cv;
using namespace std;

#include "opencv2/imgproc/imgproc.hpp"


Face::Face(Rect rect) : rect(rect) {
    xShift = yShift = 0;
    center = Point2f(rect.x + (float)rect.width / 2, rect.y + (float)rect.height / 2);
    rrect = RotatedRect(center, Size(rect.width, rect.height), 0);
}


int Face::getAngle()  { return (int)(rrect.angle - MIN_ANGLE); }
int Face::getWidth()  { return (int)(100 * rrect.size.width / rect.width) - MIN_WIDTHSCALE; }
int Face::getHeight() { return (int)(100 * rrect.size.height / rect.height) - MIN_HEIGHTSCALE; }
int Face::getX()      { return xShift - MIN_XSHIFT; }
int Face::getY()      { return yShift - MIN_YSHIFT; }


void Face::setWidth(int w)  { rrect.size.width  = (float) ((w + MIN_WIDTHSCALE)  * rect.width) / 100.0f; }
void Face::setHeight(int h) { rrect.size.height = (float) ((h + MIN_HEIGHTSCALE) * rect.height) / 100.0f; }
void Face::setAngle(int a)  { rrect.angle = a + MIN_ANGLE;  updateCenter(); }
void Face::setX(int x)      {      xShift = x + MIN_XSHIFT; updateCenter(); }
void Face::setY(int y)      {      yShift = y + MIN_YSHIFT; updateCenter(); }


void Face::drawEllipse(Mat &image, Scalar color, int thickness) {
    ellipse(image, rrect, color, thickness);
}


void Face::updateCenter() {
    float E = M_PI * rrect.angle / 180;
    float N = E - M_PI / 2;
    rrect.center = center + Point2f(cos(E),sin(E)) * xShift + Point2f(cos(N),sin(N)) * yShift;
}


bool Face::contains(int x, int y) {
    float X = x - rrect.center.x;
    float Y = y - rrect.center.y;
    float theta = -M_PI * rrect.angle / 180;
    float c = cos(theta), s = sin(theta);
    float a = 2 * (c * X - s * Y) / rrect.size.width;
    float b = 2 * (s * X + c * Y) / rrect.size.height;
    if (a * a + b * b < 1)
        return true;
    else
        return false;
}


RotatedRect Face::getRotatedRect() {
    return rrect;
}


Mat Face::getFace(const Mat &image) {
    Point2f pts[4];
    rrect.points(pts);
    Point2f src_triangle[3] = {
        pts[1],
        pts[2],
        pts[0],
    };
    Point2f dst_triangle[3] = {
        Point2f(0, 0),
        Point2f(rrect.size.width, 0),
        Point2f(0, rrect.size.height),
    };
    Mat aff(2, 3, CV_32FC1);
    aff = getAffineTransform(src_triangle, dst_triangle);
    Mat warped_image = Mat::zeros(rrect.size.height, rrect.size.width, image.type());
    warpAffine(image, warped_image, aff, warped_image.size());
    return warped_image;
}


void Face::copyToOval(Mat sourceImage, Face destFace, Mat destImage) {
    RotatedRect destrrect = destFace.getRotatedRect();
    Mat mask(destImage.rows, destImage.cols, destImage.type(), Scalar(0,0,0));
    ellipse(mask, destrrect, Scalar(255,255,255), -1);

    Mat warper(2, 3, CV_32FC1);
    Point2f srcpts[4];
    Point2f dstpts[4];
    rrect.points(srcpts);
    destrrect.points(dstpts);
    Point2f srctri[3] = { srcpts[0], srcpts[1], srcpts[2] };
    Point2f dsttri[3] = { dstpts[0], dstpts[1], dstpts[2] };
    warper = getAffineTransform(srctri, dsttri);

    Mat temp(destImage.rows, destImage.cols, destImage.type());
    warpAffine(sourceImage, temp, warper, temp.size());
    temp.copyTo(destImage, mask);
}
