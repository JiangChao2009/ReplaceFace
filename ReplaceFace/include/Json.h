#ifndef JSON_H
#define JSON_H

#include <string>
#include <map>
#include <vector>

#include <iostream>

class Json {

public:

    Json() {}
    virtual ~Json() {}

    virtual bool isDict()   { return false; }
    virtual bool isArray()  { return false; }
    virtual bool isString() { return false; }
    virtual bool isNumber() { return false; }

    virtual Json * operator[](const std::wstring &key) { return NULL; }
    virtual Json * operator[](int index)          { return NULL; }
    virtual size_t size() { return 0; }
    virtual std::string toString() { return ""; }
    virtual float value() { return 0; }

    wchar_t processUnicodeChar(const std::string &s, size_t &index);

    std::wstring processString(const std::string &s, size_t &i);

};





class JsonDict : public Json {

    std::map<std::wstring,Json*> dict;

    void init(const std::string &s, size_t &i);

public:

    JsonDict(const std::string &s);
    JsonDict(const std::string &s, size_t &i);
    ~JsonDict();

    virtual bool isDict() { return true; }
    virtual Json * operator[](const std::wstring &key) {
        return dict.at(key);
    }
    virtual size_t size() { return dict.size(); }
    std::vector<std::wstring> getKeys();

};


class JsonArray : public Json {

    std::vector<Json*> array;

public:

    JsonArray(const std::string &s, size_t &i);
    ~JsonArray();

    virtual bool isArray() { return true; }
    virtual Json * operator[](int i) {
        return array.at(i);
    }
    virtual size_t size() { return array.size(); }

};



class JsonString : public Json {

    std::wstring str;

public:

    JsonString(const std::string &s, size_t &i) : Json() {
        str = processString(s, i);
        //std::cout << "string " << toString() << std::endl;
    }

    virtual bool isString() { return true; }
    virtual std::string toString();

};



class JsonNumber : public Json {

    float number;

public:

    JsonNumber(const std::string &s, size_t &i) : Json() {
        size_t start = i;
        while (s[i] != ',' && s[i] != '}' && s[i] != ']')
            ++i;
        number = std::strtof(s.substr(start, i).c_str(), NULL);
        //std::cout << "number " << number << std::endl;
    }

    virtual bool isNumber() { return true; }
    virtual float value() { return number; }

};

#endif
