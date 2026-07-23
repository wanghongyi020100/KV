#pragma once

#include<unordered_map>
#include<string>
#include<memory>
#include<mutex>
#include<shared_mutex>

#include"Entry.h"
#include"Message.h"

template<typename Maptype>
class Entry_map:public Entry
{
private:
    Maptype ump;
    mutable std::shared_mutex share_mutex;
public:
    Entry_map(){};

    bool insert(std::string key,std::string value)override
    {
        std::unique_lock<std::shared_mutex>ul(share_mutex);
        ump[key]=std::make_shared<Message>(value);
        return ump[key]->value==value;
    }

    bool erase(std::string key)override
    {
        std::unique_lock<std::shared_mutex>ul(share_mutex);
        if(ump.find(key)==ump.end())return false;
        ump.erase(key);
        return ump.find(key)==ump.end();
    }

    bool exists(std::string key)override
    {
        std::shared_lock<std::shared_mutex>sl(share_mutex);
        return ump.find(key)!=ump.end();
    }

    std::string get(std::string key)override
    {
        std::shared_lock<std::shared_mutex>sl(share_mutex);
        if(ump.find(key)==ump.end())return "";
        return ump[key]->value;
    }

    virtual ~Entry_map()=default;
};