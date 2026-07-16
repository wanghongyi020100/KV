#include<vector>
#include<string>
#include<iostream>
#include"Entry.h"
#include"Tool.h"
#include"Message.h"

Entry::Entry(){}

void Entry::doinsert(std::string key,std::string value)
{
    if(insert(key,value))
    {
        //std::cerr<<"set "<<key<<" is "<<value<<'\n';
    }else
    {
        //std::cerr<<"set fail\n";
    }
}

void Entry::doerase(std::string key)
{
    if(erase(key))
    {
        //std::cerr<<"delete "<<key<<'\n';
    }else
    {
        //std::cerr<<"delete fail or not find\n";
    }
}

void Entry::doexists(std::string key)
{
    if(exists(key))
    {
        //std::cerr<<key<<" is exists\n";
    }else
    {
        //std::cerr<<"not find key:"<<key<<'\n';
    }
}

std::string Entry::doget(std::string key)
{
    std::string value=get(key);
    if(value!="")
    {
        //std::cerr<<"value is "<<value<<'\n';
    }else
    {
        //std::cerr<<"not find key:"<<key<<'\n';
    }
    return value;
}