#include<iostream>
#include<string>
#include"KV.h"
#include"Config.h"


int main(int argc,char *argv[])
{
    Config config(argc,argv);
    KV kv(std::make_shared<Entry_map<type_unordered_map>>(),"log.txt",config);

    while(1)
    {
        std::string s;std::getline(std::cin,s);
        kv.add(s);
        //std::cerr<<'\n';
    }
    return 0;
}