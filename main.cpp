#include<iostream>
#include<string>

#include"KV.h"
#include"Config.h"

int main(int argc,char *argv[])
{
    try
    {
        Config config(argc,argv);
        KV kv(std::make_shared<Entry_map<type_unordered_map>>(true),"log.txt",config);
        std::string s;
        while(std::getline(std::cin,s))
        {
            kv.add(s);
        }
    }catch(const std::exception &err)
    {
        std::cerr<<err.what()<<'\n';
    }
    return 0;
}