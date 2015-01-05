#include "Instagram.h"
#include "OAuth.h"
#include "Json.h"
#include "Util.h"

#include <iostream>
#include <fstream>

using namespace std;


Instagram::Instagram(string client_id, string client_secret)
    : client_id(client_id), client_secret(client_secret)
{}


Instagram::Instagram(const Instagram &ig)
    : client_id(ig.client_id), client_secret(ig.client_secret), access_token(ig.access_token)
{}


void Instagram::authorize() {
    string access_code_file("access_token");
    ifstream in(access_code_file);
    if (in) {
        string token;
        getline(in, token);
        if (testAccessToken(token)) {
            access_token = token;
            return;
        }
    }
    in.close();
    access_token = getAccessToken(client_id, client_secret); // from "OAuth.h"
    if (access_token.size() > 0) {
        ofstream out(access_code_file);
        out << access_token;
    }
}


bool Instagram::testAccessToken(string token) {
    string url = "https://api.instagram.com/v1/users/self/feed/?access_token=" + token;
    return getSuccess(url);
}


string Instagram::searchIdByUsername(string user) {
    string url = "https://api.instagram.com/v1/users/search?q=" + user
        + "&access_token=" + access_token;
    string req = request(url);
    JsonDict jd(req);
    JsonArray *ja = (JsonArray *)jd[L"data"];
    for (size_t i = 0; i < ja->size(); ++i) {
        JsonDict *d = (JsonDict *)(*ja)[i];
        string u = (*d)[L"username"]->toString();
        if (u == user)
            return (*d)[L"id"]->toString();
    }
    return "";
}


vector<ImageInfo> Instagram::getRecentMedia(string id, string &max_image_id) {
    vector<ImageInfo> infos;
    string url = "https://api.instagram.com/v1/users/" + id
        + "/media/recent?access_token=" + access_token;
    if (max_image_id.size() > 0)
        url += "&max_id=" + max_image_id;
    string req = request(url);
    JsonDict d(req);
    JsonArray *data = (JsonArray *) d[L"data"];
    if (data != NULL) {
        for (size_t i = 0; i < data->size(); ++i) {
            JsonDict *datum  = (JsonDict *) (*data)[i];
            JsonDict *images = (JsonDict *) (*datum)[L"images"];
            if (images != NULL) {
                ImageInfo info;
                JsonDict *res = (JsonDict *) (*images)[L"standard_resolution"];
                info.url = (*res)[L"url"]->toString();
                info.width  = (int) (*res)[L"width"]->value();
                info.height = (int) (*res)[L"height"]->value();
                infos.push_back(info);
            }
        }
    }
    if (data->size() > 0) {
        JsonDict *datum  = (JsonDict *) (*data)[data->size()-1];
        max_image_id = (*datum)[L"id"]->toString();
    } else {
        max_image_id = "";
    }
    return infos;
}


vector<ImageInfo> Instagram::getAllImageInfo(string id, bool verbose) {
    vector<ImageInfo> infos;
    string max_id;
    while (true) {
        vector<ImageInfo> toadd = getRecentMedia(id, max_id);
        if (toadd.size() == 0)
            break;
        infos.insert(infos.end(), toadd.begin(), toadd.end());
        if (verbose) {
            if (toadd.size() > 1)
                cout << "got locations of images " << (infos.size()-toadd.size()+1) << "-" << infos.size() << endl;
            else
                cout << "got location of image " << infos.size() << endl;
        }
    }
    return infos;
}
