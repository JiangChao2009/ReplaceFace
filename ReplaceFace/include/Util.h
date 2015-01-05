#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <map>

std::map<std::string,std::string> parse_qs(std::string query);

std::vector<std::string> split(std::string s);

std::string request(std::string url, std::string fields = std::string(""));

size_t getImage(std::string url, void *&data);

bool getSuccess(std::string url);

template<typename T>
std::string toString(T t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}


#endif
