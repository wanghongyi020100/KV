#pragma once

#include<iostream>
#include<string>
#include<utility>
#include<memory>
#include"Maptype.h"
#include"Entry_map.h"
#include"ThreadPool.h"
#include"Log.h"
#include"Config.h"

class KV
{
private:
    std::shared_ptr<Entry>p;
    std::shared_ptr<Log>log;
    
public:
    KV(std::shared_ptr<Entry>,std::string,Config);
    std::shared_ptr<ThreadPool>threadpool;
    bool add(const std::string);
};