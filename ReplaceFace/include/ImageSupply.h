#ifndef IMAGESUPPLY_H
#define IMAGESUPPLY_H

class Window;



class ImageSupply {

public:

    virtual ~ImageSupply() {}

    virtual void setWindow(Window *window) = 0;

};


#endif
