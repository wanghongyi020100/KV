# KVS

一个基于 **C++17** 实现的轻量级键值存储（Key-Value Store），支持 `set`、`get`、`del`、`exists` 四类命令，采用 **WAL（Write-Ahead Logging）** 实现数据持久化与崩溃恢复，并通过线程池支持并发任务处理。

---

## 项目特点

- 支持 `set`、`get`、`del`、`exists` 四类命令
- 基于 WAL（Write-Ahead Logging）实现持久化与启动恢复
- 使用线程池并发执行任务
- 使用 `std::shared_mutex` 实现读写并发控制
- 存储层抽象，支持 `std::unordered_map` 与 `std::map` 两种底层实现
- 支持命令大小写无关及参数合法性校验
- 提供随机正确性测试、性能测试及恢复一致性测试

---

## 技术栈

- C++17
- STL
  - thread
  - future
  - mutex
  - shared_mutex
  - map
  - unordered_map
- ThreadPool
- WAL（Write-Ahead Logging）
- Makefile
- Linux

---

## 目录结构

```text
.
├── Config/          运行参数配置
├── Entry/           存储层抽象及 map/unordered_map 实现
├── KVS/             KV 业务逻辑
├── Log/             WAL 写入与恢复
├── Message/         数据对象
├── ThreadPool/      线程池
├── Tool/            工具函数
├── kv_test/         随机测试、性能测试、恢复测试
├── main.cpp         程序入口
└── Makefile
```

---

## 构建

```bash
make
```

生成：

```text
kv      主程序
kvt1    unordered_map 测试程序
kvt2    map 测试程序
```

---

## 运行

默认线程数：

```bash
./kv
```

指定线程数：

```bash
./kv 8
```

启动后，从标准输入读取命令，例如：

```text
set name Alice
get name
exists name
del name
```

---

## WAL 持久化

所有写操作都会追加写入 WAL 日志：

```text
set name Alice
set age 20
del age
```

程序启动时自动读取日志并重放写操作，实现数据恢复。

---

## 测试

项目包含以下测试：

### 1. 随机正确性测试

随机生成大量合法、非法命令，验证命令解析及执行正确性。

---

### 2. 两种容器实现一致性测试

分别基于：

- std::unordered_map
- std::map

执行完全相同的数据集，比较最终 WAL 是否一致。

---

### 3. 性能测试

统计：

- 总耗时
- QPS
- P50
- P90
- P95
- P99

用于比较不同底层容器的性能。

---

### 4. 多线程测试

线程池并发提交随机请求，验证：

- 多线程执行稳定性
- 日志一致性
- 并发访问正确性

---

### 5. WAL 恢复测试

使用已有 WAL 日志启动程序，验证恢复后的数据是否正确。

---

### 6. 崩溃恢复测试

运行过程中使用：

```bash
kill -9 <pid>
```

模拟程序异常退出，重新启动后验证 WAL 能否恢复数据。

---

### 7. 恢复一致性校验

恢复完成后，对生成日志计算 SHA256：

```bash
sha256sum test_1.txt test_2.txt
```

验证两种底层实现恢复结果一致。

---

## 项目设计

```text
客户端请求
        │
        ▼
    ThreadPool
        │
        ▼
 KV::set/get/del/exists
        │
        ├──────────────┐
        ▼              ▼
      Entry           WAL
(map/unordered_map)   Log
```

---

## 后续优化方向

- Snapshot（快照）
- LRU Cache
- Batch Write
- 数据压缩
- TCP 网络接口
- 更细粒度并发控制

---

## 简历描述

> 基于 C++17 实现轻量级键值存储系统，支持 `set/get/del/exists` 指令；基于 WAL（Write-Ahead Logging）实现持久化与崩溃恢复，采用线程池和 `std::shared_mutex` 支持并发访问；抽象存储层支持 `std::unordered_map` 与 `std::map` 两种底层实现，并完成随机正确性测试、多线程压力测试、性能测试及崩溃恢复验证。