#include<memory>
#include<string>
#include<system_error>

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
        throw std::system_error(errno,std::generic_category(),"KV build faild");
    }
}

void KV::add(const std::string s)
{
    threadpool->add([this,s](){log->add(p,s);});
}