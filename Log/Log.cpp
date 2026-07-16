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
        std::cerr<<"open file fail"<<'\n';
        return false;
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
            //std::cerr<<"y to continue n to return else to skip all fail log";
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

    //std::cerr<<"this is "<<s<<'\n';

    int n=s.length();
    std::vector<std::string>v;
    for(int i=0;i<n;i++)
    {
        if(s[i]==' ')continue;
        int r=i;
        while(r+1<n&&s[r+1]!=' ')r++;
        v.push_back(s.substr(i,r-i+1));
        i=r;
        //std::cerr<<v.back()<<'\n';
    }
    int m=v.size();
    if(m==0)
    {
        //std::cerr<<"count is "<<m<<" not accept"<<'\n';
        return false;
    }
    Tool::to_a_ascii(v[0]);

    if(m==1)
    {
        //std::cerr<<"count is "<<m<<" not accept"<<'\n';
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
                std::cerr<<"del log fail"<<'\n';
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
            //std::cerr<<"count is "<<m<<" not accept"<<'\n';
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
                std::cerr<<"set log fail"<<'\n';
                return false;
            }
        }else
        {
            //std::cerr<<"count is "<<m<<" not accept"<<'\n';
            return false;
        }
    }else
    {
        //std::cerr<<"count is "<<m<<" not accept"<<'\n';
        return false;
    }
    return true;
}