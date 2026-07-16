#pragma once

#include<string>
#include"Message.h"

class Entry
{
public:
    Entry();
    std::string key;
    std::string value;

    void doinsert(std::string,std::string);
    void doerase(std::string);
    void doexists(std::string);
    std::string doget(std::string);
//protected:
    virtual bool insert(std::string,std::string)=0;
    virtual bool erase(std::string)=0;
    virtual bool exists(std::string)=0;
    virtual std::string get(std::string)=0;
};