#ifndef INSTAGRAM_H
#define INSTAGRAM_H

#include "ImageSupply.h"

#include <string>
#include <vector>



class ImageInfo {
public:
    std::string url;
    int width, height;
};




class Instagram {

    std::string client_id, client_secret, access_token;

public:

    Instagram(std::string client_id, std::string client_secret);
    Instagram(const Instagram &ig);

    void authorize();
    
    std::string searchIdByUsername(std::string user);
    std::vector<ImageInfo> getRecentMedia(std::string id, std::string &max_image_id);
    std::vector<ImageInfo> getAllImageInfo(std::string id, bool verbose = true);

private:

    bool testAccessToken(std::string token);

};

#endif
