#include "Json.h"

#include <cstdlib>
#include <iostream>


using namespace std;



wchar_t Json::processUnicodeChar(const string &s, size_t &index) {
    string numstr = s.substr(index+2, index+6);
    index += 6;
    unsigned int n = strtol(numstr.c_str(), NULL, 16);
    if (n < 256)
        return n;
    unsigned int ret = 0, shift = 0;
    while (n >= (unsigned)(0x80 >> shift)) {
        ret += (0x80 | (n & 0x3F)) << (8 * shift);
        ++shift;
        n /= 0x40;
    }
    ret += (0x100 - (0x100 >> (shift+1)) + n) << (8 * shift);
    return ret;
}


wstring Json::processString(const string &s, size_t &i) {
    if (s[i] != '"') {
        cerr << "Error: Not a string (index " << i << ")" << endl;
        exit(-1);
    }
    ++i;
    wstring ret;
    while (s[i] != '"') {
        if (s[i] == '\\') {
            switch (s[i+1]) {
            case '"':  ret += '"';  i += 2; break;
            case '\\': ret += '\\'; i += 2; break;
            case '/':  ret += '/';  i += 2; break;
            case 'b':  ret += '\b'; i += 2; break;
            case 'f':  ret += '\f'; i += 2; break;
            case 'n':  ret += '\n'; i += 2; break;
            case 'r':  ret += '\r'; i += 2; break;
            case 't':  ret += '\t'; i += 2; break;
            case 'u':  ret += processUnicodeChar(s, i); break;
            default:
                cerr << "Error: Can't process escape character: \\"
                    << s[i+1] << " at index " << i << endl;
                exit(-1);
            }
        } else
            ret += s[i++];
    }
    ++i;
    return ret;
}



JsonDict::JsonDict(const string &s) : Json() {
    size_t i = 0;
    init(s, i);
}

JsonDict::JsonDict(const string &s, size_t &i) : Json() {
    init(s, i);
}

void JsonDict::init(const string &s, size_t &i) {
    if (s[i] == '{') {
        ++i;
        while (s[i] != '}') {
            wstring key = processString(s, i);
            if (s[i] != ':') {
                cerr << "Error: ':' expected (index " << i << ")" << endl;
                exit(-1);
            }
            ++i;
            string skey;
            for (unsigned i = 0; i < key.size(); ++i)
                skey += key[i];
            switch (s[i]) {
            case '{': dict[key] = new JsonDict(s, i);   break;
            case '[': dict[key] = new JsonArray(s, i);  break;
            case '"': dict[key] = new JsonString(s, i); break;
            default:  dict[key] = new JsonNumber(s, i); break;
            }
            if (s[i] == ',')
                ++i;
        }
    } else {
        cerr << s << endl;
        cerr << "Error: '{' expected (index " << i << ")" << endl;
        exit(-1);
    }
    ++i;
}


JsonDict::~JsonDict() {
    for (auto it = dict.begin(); it != dict.end(); ++it)
        delete it->second;
}


vector<wstring> JsonDict::getKeys() {
    vector<wstring> ret;
    for (auto it = dict.begin(); it != dict.end(); ++it)
        ret.push_back(it->first);
    return ret;
}




JsonArray::JsonArray(const string &s, size_t &i) : Json() {
    if (s[i] == '[') {
        ++i;
        while (s[i] != ']') {
            switch (s[i]) {
            case '{': array.push_back(new JsonDict(s, i));   break;
            case '[': array.push_back(new JsonArray(s, i));  break;
            case '"': array.push_back(new JsonString(s, i)); break;
            default:  array.push_back(new JsonNumber(s, i)); break;
            }
            if (s[i] == ',')
                ++i;
        }
    } else {
        cerr << "Error: '[' expected (index " << i << ")" << endl;
        exit(-1);
    }
    ++i;
}



JsonArray::~JsonArray() {
    for (auto it = array.begin(); it != array.end(); ++it)
        delete *it;
}



string JsonString::toString() {
    string ret;
    for (size_t i = 0; i < str.size(); ++i)
        ret += str[i];
    return ret;
}
