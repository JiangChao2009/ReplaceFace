#ifndef INSTAGRAMIMAGESUPPLY_H
#define INSTAGRAMIMAGESUPPLY_H

#include "ImageSupply.h"
#include "Instagram.h"

#include "opencv2/highgui/highgui.hpp"

#include <string>


class InstagramImageSupply : public ImageSupply {

    Instagram instagram;
    Window *window;
    std::vector<ImageInfo> infos;
    int imageIndex;
    std::string trackbar;
    std::string max_id;

public:

    InstagramImageSupply(Instagram instagram);

    bool setUsername(std::string username);

    bool empty() { return infos.empty(); }

    void setWindow(Window *window);
    Window * getWindow() { return window; }

    cv::Mat downloadImage(size_t index);

    static void imageCallback(int position, void *value);

};

#endif
