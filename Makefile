.PHONY:clean kvt1 kvt2

cxx=g++
cxxflag=-std=c++17 -Wall -Wextra -I. -IEntry -IMessage -ITool -ILog -IKVS\
-IThreadPool -Ikv_test -IConfig

debug?=1
ifeq ($(debug),1)
	cxxflag+=-g -O0
else
	cxxflag+=-O2 -DNDEBUG
endif

threadflag=-pthread 
target=kv
test_target1=kvt1
test_target2=kvt2

srcs=Tool/Tool.cpp\
	 Message/Message.cpp\
	 Entry/Entry.cpp\
	 Log/Log.cpp\
	 KVS/KV.cpp\
	 ThreadPool/ThreadPool.cpp\
	 Config/Config.cpp

main_srcs=$(srcs) main.cpp
test_srcs1=$(srcs) kv_test/test_1.cpp
test_srcs2=$(srcs) kv_test/test_2.cpp 

all:$(target) $(test_target1) $(test_target2)

$(target):$(main_srcs:.cpp=.o)
	$(cxx) $(main_srcs:.cpp=.o) -o $(target) $(threadflag)

$(test_target1):$(test_srcs1:.cpp=.o)
	$(cxx) $(test_srcs1:.cpp=.o) -o $(test_target1) $(threadflag)
$(test_target2):$(test_srcs2:.cpp=.o)
	$(cxx) $(test_srcs2:.cpp=.o) -o $(test_target2) $(threadflag)
%.o:%.cpp
	$(cxx) $(cxxflag) $(threadflag) -c $< -o $@

clean:
	rm -f $(target) $(test_target1) $(test_target2) $(srcs:.cpp=.o) $(main_srcs:.cpp=.o)\
		  $(test_srcs1:.cpp=.o) $(test_srcs2:.cpp=.o)