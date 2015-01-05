#include "OAuth.h"
#include "SimpleServer.h"
#include "Json.h"
#include "Util.h"

#include <curl/curl.h>
#include <sstream>
#include <vector>
#include <map>

#include <iostream>

#define PORT 8000

using namespace std;



string getFirstLine(string s) {
    string ret;
    stringstream ss(s);
    getline(ss, ret, '\n');
    return ret;
}



string getAccessToken(string client_id, string client_secret) {
    SimpleServer ss;
    if (ss.bindWithPort(PORT) != PORT) {
        cerr << "Error: Unable to open socket" << endl;
        exit(-1);
    }
    CURL *curl = curl_easy_init();
    string redirect_uri = "http://localhost:" + toString<short>(PORT);
    redirect_uri = curl_easy_escape(curl, redirect_uri.c_str(), redirect_uri.size());
    curl_easy_cleanup(curl);
    
    cout << endl << "Open a web browser to the link below to authorize this application"
        << endl << endl << "https://api.instagram.com/oauth/authorize/?client_id=" << client_id
        << "&redirect_uri=" << redirect_uri << "&response_type=code" << endl << endl;
    
    if (!ss.listenAndConnect()) {
        cerr << "Error: Something went wrong listening on the socket" << endl;
        exit(-1);
    }
    string req = ss.readClient();
    req = getFirstLine(req);
    vector<string> chunks = split(req);
    string querystr = chunks[1];

    size_t start = 0;
    if (querystr[start] == '/') ++start;
    if (querystr[start] == '?') ++start;
    map<string,string> query = parse_qs(querystr.substr(start));

    if (query.find("code") == query.end()) {
        cerr << "Error: Couldn't get authorization code" << endl;
        exit(-1);
    }
    string fields = "client_id=" + client_id
        + "&client_secret=" + client_secret
        + "&grant_type=authorization_code"
        + "&redirect_uri=" + redirect_uri
        + "&code=" + query["code"];
    string theresponse = request("https://api.instagram.com/oauth/access_token", fields);
    JsonDict jd(theresponse);
    JsonDict *ud = (JsonDict *)jd[L"user"];
    string username = (*ud)[L"username"]->toString();
    ss.writeClient("HTTP/1.1 302 Found\r\nLocation: http://instagram.com/" + username);
    ss.closeClient();
    return jd[L"access_token"]->toString();
}
