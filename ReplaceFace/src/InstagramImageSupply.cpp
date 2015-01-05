#include "InstagramImageSupply.h"
#include "Window.h"
#include "Util.h"

#include <iostream>

using namespace std;
using namespace cv;



InstagramImageSupply::InstagramImageSupply(Instagram instagram)
    : instagram(instagram), window(NULL), infos(), imageIndex(0), trackbar("Image Index")
{}


bool InstagramImageSupply::setUsername(string username) {
    string id = instagram.searchIdByUsername(username);
    if (id.size() <= 0) {
        cerr << "Error: Could not find user '" << username << "'" << endl;
        return false;
    }
    cout << endl << "Collecting information on " << username << "'s instagram feed" << endl;
    infos = instagram.getAllImageInfo(id, true);
    return !infos.empty();
}



void InstagramImageSupply::setWindow(Window *window) {
    this->window = window;
    if (window != NULL) {
        createTrackbar(trackbar, window->getControlWindow(), NULL, infos.size(),
                InstagramImageSupply::imageCallback, (void *)this);
        InstagramImageSupply::imageCallback(0, (void *)this);
    }
}



Mat InstagramImageSupply::downloadImage(size_t index) {
    ImageInfo info = infos[index];
    void *imgdata = NULL;
    getImage(info.url, imgdata); // from "Util.h"
    Mat imgbuf(Size(info.width, info.height), CV_8UC3, imgdata);
    Mat img = imdecode(imgbuf, CV_LOAD_IMAGE_COLOR);
    return img;
}



void InstagramImageSupply::imageCallback(int position, void *value) {
    InstagramImageSupply *iis = (InstagramImageSupply *)value;
    Window *w = iis->getWindow();
    if (w != NULL) {
        Mat m = iis->downloadImage(position);
        w->setImage(m);
    }
}
