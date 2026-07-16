#include<string>
#include<utility>
#include<vector>
#include"Tool.h"

std::string Tool::g_string_no32char(std::string &c,int l,int r)//[),去除空格
{
    while(l+1<r&&c[l]==' ')l++;
    r--;
    if(r<l)return "";
    while(r-1>=l&&c[r]==' ')r--;
    return c.substr(l,r-l);
}

void Tool::to_a_ascii(std::string &s)//全小写
{
    for(auto &i:s)if(i>='A'&&i<='Z')i+=32;
}