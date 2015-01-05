#include "SingleImageSupply.h"

#include "Window.h"

using namespace std;
using namespace cv;


SingleImageSupply::SingleImageSupply(Mat m) : ImageSupply() {
    m.copyTo(this->m);
}


void SingleImageSupply::setWindow(Window *w) {
    w->setImage(m);
}
