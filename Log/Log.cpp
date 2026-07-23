#include<iostream>
#include<fstream>
#include<memory>
#include<string>
#include<vector>

#include"Entry.h"
#include"Tool.h"
#include"Log.h"

Log::Log(std::string f):log_file(f){}

bool Log::start(std::shared_ptr<Entry>p)
{
    std::ifstream file(log_file);
    if(!file.is_open())
    {
        std::ofstream create_file(log_file);
        if(!create_file.is_open())
        {
            std::cerr<<"open file fail"<<'\n';
            return false;
        }
        return true;
    }

    std::string s;
    std::vector<std::string>v;
    int idx=0;
    while(std::getline(file,s))
    {
        idx++;
        v.clear();
        int n=s.length();
        for(int i=0;i<n;i++)
        {
            if(s[i]==' ')continue;
            int r=i;
            while(r+1<n&&s[r+1]!=' ')r++;
            v.push_back(s.substr(i,r-i+1));
            i=r;
        }
        int m=v.size();
        if(m==2&&v[0]=="del")p->erase(v[1]);
        else if(m==3&&v[0]=="set")p->insert(v[1],v[2]);
        else
        {
            std::cerr<<"log start fail at line:"<<idx<<'\n';
            return false;
        }
    }
    return true;
}

bool Log::add(std::shared_ptr<Entry>p,const std::string &s)
{
    std::ofstream file(log_file,std::ios::out|std::ios::app);
    if(!file.is_open())
    {
        std::cerr<<"open file fail:"<<'\n';
        return false;
    }

    int n=s.length();
    std::vector<std::string>v;
    for(int i=0;i<n;i++)
    {
        if(s[i]==' ')continue;
        int r=i;
        while(r+1<n&&s[r+1]!=' ')r++;
        v.push_back(s.substr(i,r-i+1));
        i=r;
    }
    int m=v.size();
    if(m==0)
    {
        return false;
    }
    Tool::to_a_ascii(v[0]);

    if(m==1)
    {
        return false;
    }else if(m==2)
    {
        if(v[0]=="del")
        {
            {
                std::lock_guard<std::mutex>lg(log_mutex);
                file<<"del "<<v[1]<<'\n';
                file.flush();
            }
            
            if(file)p->doerase(v[1]);
            else
            {
                return false;
            }
        }else if(v[0]=="exists")
        {
            p->doexists(v[1]);
        }else if(v[0]=="get")
        {
            p->doget(v[1]);
        }else
        {
            return false;
        }
    }else if(m==3)
    {
        if(v[0]=="set")
        {
            {
                std::lock_guard<std::mutex>lg(log_mutex);
                file<<"set "<<v[1]<<' '<<v[2]<<'\n';
                file.flush();
            }
            
            if(file)p->doinsert(v[1],v[2]);
            else
            {
                return false;
            }
        }else
        {
            return false;
        }
    }else
    {
        return false;
    }
    return true;
}
