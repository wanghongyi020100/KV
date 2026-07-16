#pragma once

#include<thread>
#include<queue>
#include<vector>
#include<memory>
#include<future>
#include<functional>
#include<mutex>
#include<condition_variable>
#include<type_traits>

class ThreadPool
{
private:
    std::vector<std::thread>work;
    std::queue<std::function<void()>>q;
    std::mutex m;
    std::condition_variable cv;
    bool stop;

    //测试:
    int cnt_task;
    std::condition_variable test_fincv;
public:
    ThreadPool(int n);
    ~ThreadPool();

    void wait_test();
    
    template<typename F,typename ...Args>
    auto add(F &&f,Args &&...args)->std::future<std::invoke_result_t<F,Args...>>
    {
        using f_type=std::invoke_result_t<F,Args...>;
        auto p=std::make_shared<std::packaged_task<f_type()>>
        (std::bind(std::forward<F>(f),std::forward<Args>(args)...));
        std::future<f_type>res=p->get_future();
        {
            std::lock_guard<std::mutex>lg(m);
            cnt_task++;
            q.push([p,this]()
            {
                (*p)();
                {
                    std::lock_guard<std::mutex>lg(m);
                    cnt_task--;
                    if(cnt_task==0)test_fincv.notify_one();
                }
            });
        }
        cv.notify_one();
        return res;
    }
};