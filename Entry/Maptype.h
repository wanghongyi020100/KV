#pragma once

#include<map>
#include<unordered_map>
#include<string>
#include<memory>
#include"Message.h"

using type_unordered_map=std::unordered_map<std::string,std::shared_ptr<Message>>;
using type_map=std::map<std::string,std::shared_ptr<Message>>;