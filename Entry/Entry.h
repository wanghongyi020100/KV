#pragma once

#include<string>
#include"Message.h"

class Entry
{
public:
    bool print=false;
    Entry(bool ckprint=false);

    //保留可能的处理
    void doinsert(std::string,std::string);
    void doerase(std::string);
    void doexists(std::string);
    std::string doget(std::string);

    virtual bool insert(std::string,std::string)=0;
    virtual bool erase(std::string)=0;
    virtual bool exists(std::string)=0;
    virtual std::string get(std::string)=0;
};