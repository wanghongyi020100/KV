#include<iostream>
#include<string>
#include<memory>
#include<unistd.h>
#include<random>
#include<fstream>
#include<bits/stdc++.h>
#include"Maptype.h"
#include"KV.h"
#include"Config.h"
using namespace std;

std::mt19937 Rand(1123);

const int CNT=10000000;
vector<string>vs{"get","set","del","exists"};

void get(std::vector<std::string> &in)
{
    int get=1,set=1,del=1,exists=1,fail=1;
    int sum=get+set+del+exists+fail;
    int n=CNT;//std::cin>>n;
    for(int i=1;i<=n;i++)
    {
        std::string res="";
        int t=Rand()%sum,sk=Rand()%3+1,sv=Rand()%3+1;
        if(t<get)
        {
            res+="get ";
            for(int j=1;j<=sk;j++)res+=Rand()%26+'a';
            //res+="\n";
        }else if(t<get+set)
        {
            res+="set ";
            for(int j=1;j<=sk;j++)res+=Rand()%26+'a';
            res+=" ";
            for(int j=1;j<=sv;j++)res+=Rand()%26+'a';
            //res+="\n";
        }
        else if(t<get+set+del)
        {
            res+="del ";
            for(int j=1;j<=sk;j++)res+=Rand()%26+'a';
            //res+="\n";
        }else if(t<get+set+del+exists)
        {
            res+="exists ";
            for(int j=1;j<=sk;j++)res+=Rand()%26+'a';
            //res+="\n";
        }
        else if(t<get+set+del+exists+fail)
        {
            int ch=Rand()%5;
            if(ch!=4)
            {
                for(const auto &i:vs[ch])
                {
                    int ck=Rand()%2;
                    res+=ck?i:i-32;
                }
                res+=' ';
                int kv=Rand()%4;
                for(int i=1;i<=kv;i++)
                {
                    int len=4,ck=Rand()%2;
                    for(int i=1;i<=len;i++)res+=Rand()%26+(ck?'a':'A');
                    res+=' ';
                }
            }else
            {
                int kv=Rand()%4;
                for(int i=1;i<=kv;i++)
                {
                    int len=4,ck=Rand()%2;
                    for(int i=1;i<=len;i++)res+=Rand()%26+(ck?'a':'A');
                    res+=' ';
                }
            }
        }
        in.push_back(res);
    }
}

int main(int argc,char *argv[])
{
    Config config(argc,argv);
    KV kv2(std::make_shared<Entry_map<type_unordered_map>>(),"kv_test/test_1.txt",config);
    std::vector<std::string>in;
    std::vector<long long>v2;
    get(in);
    std::cerr<<"build ok"<<'\n';

    std::ofstream out("kv_test/compare.txt",std::ios::out|std::ios::app);
    auto start=std::chrono::steady_clock::now();
    auto end=std::chrono::steady_clock::now();
    auto cost=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();

    std::cerr<<"out ok"<<'\n';
    start=std::chrono::steady_clock::now();
    for(int i=0;i<CNT;i++)
    {
        kv2.add(in[i]);
        if(i%10000==0)std::cerr<<"1:"<<i<<'\n';
    }
    kv2.threadpool->wait_test();
    end=std::chrono::steady_clock::now();
    cost=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    
    out<<"1 all time:"<<cost<<" QPS:"<<CNT*1000000000ll/cost<<'\n';
    return 0;
}