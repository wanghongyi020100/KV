#include<thread>
#include<queue>
#include<vector>
#include<memory>
#include<future>
#include<functional>
#include<mutex>
#include<condition_variable>
#include<atomic>
#include<type_traits>
#include"ThreadPool.h"
#include<iostream>

ThreadPool::ThreadPool(int n):stop(false),cnt_task(0)
{
    for(int i=1;i<=n;i++)
    {
        work.emplace_back([this]()
        {
            while(true)
            {
                std::function<void()>f;
                {
                    std::unique_lock<std::mutex>ul(m);
                    cv.wait(ul,[this]()->bool{return stop||!q.empty();});
                    if(stop&&q.empty())break;
                    f=std::move(q.front());
                    q.pop();
                }
                f();
            }
        });
    }
}

void ThreadPool::wait_test()
{
    std::unique_lock<std::mutex>ul(m);
    test_fincv.wait(ul,[this]()->bool{return cnt_task==0;});
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex>lg(m);
        stop=true;
    }
    cv.notify_all();
    for(auto &i:work)if(i.joinable())i.join();
}