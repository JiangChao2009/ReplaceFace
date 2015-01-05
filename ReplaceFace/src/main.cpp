#include "Util.h"
#include "SourceWindow.h"
#include "DestinationWindow.h"
#include "Instagram.h"
#include "InstagramImageSupply.h"
#include "SingleImageSupply.h"

#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>

using namespace std;
using namespace cv;


string client_id = "";
string client_secret = "";


Mat takePicture(string pictureName);
void instaInit(Instagram *&instagram);



int main(int argc, char **argv) {

    ImageSupply *sourceSupply = NULL;
    ImageSupply *destinationSupply = NULL;
    Instagram *instagram = NULL;

    Mat m;
    int option = 0;
    while ( (option = getopt(argc, argv, "f:i:F:I:tT")) != -1) {
        
        if (sourceSupply != NULL && destinationSupply != NULL)
            break;

        switch (option) {
            case 'f':
                m = imread(optarg);
                sourceSupply = new SingleImageSupply(m);
                break;
            case 'F':
                m = imread(optarg);
                destinationSupply = new SingleImageSupply(m);
                break;
            case 'i':
                instaInit(instagram);
                sourceSupply = new InstagramImageSupply(*instagram);
                if (!((InstagramImageSupply *)sourceSupply)->setUsername(optarg))
                    return -1;
                break;
            case 'I':
                instaInit(instagram);
                destinationSupply = new InstagramImageSupply(*instagram);
                if (!((InstagramImageSupply *)destinationSupply)->setUsername(optarg))
                    return -1;
                break;
            case 't':
                m = takePicture("Source Image");
                sourceSupply = new SingleImageSupply(m);
                break;
            case 'T':
                m = takePicture("Destination Image");
                destinationSupply = new SingleImageSupply(m);
                break;
            default:
                cerr << "Unknown option: " << ((char)option) << endl;
                return -1;
                break;
        }
    }

    if (sourceSupply == NULL || destinationSupply == NULL) {
        cerr << "Usage: source and destination images suppliers must be specified"
            << endl << endl
            << "\tSource image:"
            << endl << "\t\t-t                       take a picture"
            << endl << "\t\t-f <image-file>          use an image file"
            << endl << "\t\t-i <instagram-username>  use an instagram account" << endl
            << endl << "\tDestination image:"
            << endl << "\t\t-T                       take a picture"
            << endl << "\t\t-F <image-file>          use an image file"
            << endl << "\t\t-I <instagram-username>  use an instagram account" << endl << endl;
        return -1;
    }

    Window::setClassifier("cascade/haarcascade_frontalface_alt.xml");
    SourceWindow sw("Source Image", "Source Image Controls");
    DestinationWindow dw("Destination Image (Hit 's' to save image)", "Destination Image Controls");
    sourceSupply->setWindow(&sw);
    destinationSupply->setWindow(&dw);
    dw.setSource(&sw);
    sw.setDestination(&dw);

    bool done = false;
    while (!done) {
        char c = waitKey(33);
        switch (c) {
            case 27: done = true; break;
            case 's': dw.saveImage(toString<time_t>(time(NULL)) + ".jpg"); break;
        }
    }

    return 0;
}




Mat takePicture(string pictureName) {
    string title("Press any key to take " + pictureName);
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "No video capture device found" << endl;
        exit(-1);
    }
    namedWindow(title, CV_WINDOW_AUTOSIZE);
    while (true) {
        Mat m;
        cap >> m;
        imshow(title, m);
        if (waitKey(30) >= 0) {
            destroyWindow(title);
            return m;
        }
    }
}


void instaInit(Instagram *&instagram) {
    if (instagram == NULL) {
        instagram = new Instagram(client_id, client_secret);
        instagram->authorize();
    }
}
