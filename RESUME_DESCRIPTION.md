# 简历描述

基于 C++17 实现轻量级键值存储系统，支持 `set/get/del/exists` 指令、日志持久化恢复和线程池并发处理；抽象存储层并支持 `std::map` 与 `std::unordered_map` 两种底层容器，在百万级随机指令基准测试中，`unordered_map` 版本 QPS 最高约 49 万。
