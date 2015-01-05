#ifndef SINGLEIMAGESUPPLY_H
#define SINGLEIMAGESUPPLY_H

#include "ImageSupply.h"

#include <string>

#include "opencv2/highgui/highgui.hpp"


class SingleImageSupply : public ImageSupply {

    cv::Mat m;

public:

    SingleImageSupply(cv::Mat m);
    virtual ~SingleImageSupply() {}

    virtual void setWindow(Window *w);

};


#endif
