#pragma once

#include<iostream>
#include<memory>
#include<string>
#include<mutex>
#include"Entry.h"

class Log
{
public:
    Log(std::string);
    std::string log_file;
    std::mutex log_mutex;
    bool start(std::shared_ptr<Entry>);
    bool add(std::shared_ptr<Entry>,const std::string &);
};