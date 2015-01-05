#include "Util.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <curl/curl.h>

using namespace std;



map<string,string> parse_qs(string query) {
	map<string,string> qmap;
	while (query.size() > 0) {
		size_t amp = query.find("&");
		size_t eq = query.find("=");
		if (amp == string::npos) {
			if (eq == string::npos)
				qmap[query] = "";
            else
				qmap[query.substr(0, eq)] = query.substr(eq + 1);
           
			break;
		} else {
            qmap[query.substr(0, eq)] = query.substr(eq + 1, amp - eq - 1);
			query = query.substr(amp + 1);
		}
	}
	return qmap;
}



vector<string> split(string s) {
    vector<string> ret;
    stringstream ss(s);
    string str;
    while (ss >> str)
        ret.push_back(str);
    return ret;
}



typedef struct buffer {
    char *buffer;
    size_t buflen;
    size_t pos;
} buffer;

size_t write_to_string(void *ptr, size_t size, size_t nmemb, void *s) {
    buffer *b = (buffer *)s;
    while (b->buflen - 1 <= b->pos + size * nmemb) {
        void *newbuf = malloc(2 * b->buflen);
        if (newbuf == NULL) {
            cerr << "Error: bad malloc" << endl;
            free(b->buffer);
            exit(-1);
        }
        memcpy(newbuf, (void *)b->buffer, b->buflen);
        free(b->buffer);
        b->buffer = (char *)newbuf;
        b->buflen *= 2;
    }
    memcpy((void *)(b->buffer + b->pos), ptr, size * nmemb);
    b->pos += size * nmemb;
    return size * nmemb;
}

#define INITBUFLEN 1024

string request(string url, string fields) {
    buffer datums;
    datums.buflen = INITBUFLEN;
    datums.pos = 0;
    datums.buffer = (char *)malloc(INITBUFLEN * sizeof(char));
    if (datums.buffer == NULL) {
        cerr << "Error: Unable to allocate space for HTTP request" << endl;
        exit(-1);
    }
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        cerr << "Error: Unable to initialize cURL" << endl;
        return string();
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (fields.size() > 0)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&datums);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    int result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        cerr << "Error: HTTP code: " << result << endl;
        return string();
    }
    curl_easy_cleanup(curl);
    string ret(datums.buffer);
    free(datums.buffer);
    return ret;
}


size_t getImage(string url, void *&data) {
    buffer datums;
    datums.buflen = INITBUFLEN;
    datums.pos = 0;
    datums.buffer = (char *)malloc(INITBUFLEN * sizeof(char));
    if (datums.buffer == NULL) {
        cerr << "Error: Unable to allocate space for HTTP request" << endl;
        exit(-1);
    }
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        cerr << "Error: Unable to initialize cURL" << endl;
        return 0;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&datums);
    int result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        cerr << "Error: HTTP code: " << result << endl;
        return 0;
    }
    curl_easy_cleanup(curl);
    data = (void *)datums.buffer;
    return datums.pos;
}



bool getSuccess(string url) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        cerr << "Error: Unable to initialize cURL" << endl;
        return false;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    int result = curl_easy_perform(curl);
    return result == CURLE_OK;
}
