#include<string>
#include"Config.h"

Config::Config(int n,char *c[])
{
    if(n==1)return;
    CNT_THREAD=std::stoi(c[1]);
}