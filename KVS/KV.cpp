#include<memory>
#include<string>
#include"Log.h"
#include"ThreadPool.h"
#include"KV.h"
#include"Maptype.h"
#include"Entry.h"
#include"Config.h"

KV::KV(std::shared_ptr<Entry>entry,std::string s,Config config)
{
    p=std::move(entry);
    log=std::make_shared<Log>(s);
    threadpool=std::make_shared<ThreadPool>(config.CNT_THREAD);
    if(!log->start(p))
    {
        std::cerr<<"KV build fail\n";
        return;
    }
}

bool KV::add(const std::string s)
{
    return log->add(p,s);
}