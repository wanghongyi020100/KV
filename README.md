# KVS

一个基于 **C++17** 实现的轻量级键值存储系统，支持 `set`、`get`、`del`、`exists` 四类命令。项目通过 WAL（Write-Ahead Logging）实现数据持久化与启动恢复，并使用线程池提交请求任务，支持对 `std::unordered_map` 与 `std::map` 两种底层容器进行一致性和性能对比。

---

## 项目特点

- 支持 `set`、`get`、`del`、`exists` 四类基础命令
- 基于 WAL 实现写操作持久化和启动恢复
- 使用线程池异步提交任务
- 使用 `std::shared_mutex` 实现读写并发控制
- 存储层抽象，支持 `std::unordered_map` 和 `std::map` 两种实现
- 支持命令大小写无关解析和参数合法性校验
- 提供随机正确性测试、一致性测试和性能测试

---

## 技术栈

- C++17
- STL：`thread`、`future`、`mutex`、`shared_mutex`、`map`、`unordered_map`
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
├── kv_test/         随机测试与性能测试
├── main.cpp         程序入口
└── Makefile
```

---

## 构建

默认构建为调试模式，包含 `-g -O0`：

```bash
make
```

构建发布模式：

```bash
make clean
make debug=0
```

生成目标：

```text
kv      主程序
kvt1    unordered_map 测试程序
kvt2    map 测试程序
```

切换调试/发布模式前建议先执行 `make clean`，避免复用旧的中间产物。

清理构建产物：

```bash
make clean
```

---

## 运行

默认线程数为 2：

```bash
./kv
```

指定线程数：

```bash
./kv 4
```

启动后从标准输入读取命令，例如：

```text
set name Alice
get name
exists name
del name
```

---

## WAL 持久化

所有写操作都会先追加写入 WAL 日志：

```text
set name Alice
set age 20
del age
```

程序启动时会自动读取 WAL 并重放写操作，恢复历史数据。

说明：当前实现使用 `file.flush()` 将 C++ 文件缓冲区刷新到操作系统缓存，可降低进程异常退出导致的数据丢失风险；若需要抵抗系统崩溃或断电级别的问题，还需要结合 `fsync` 等系统级落盘机制。

---

## 测试

项目包含以下测试方向：

### 1. 随机正确性测试

随机生成大量合法和非法命令，验证命令解析、参数校验和执行逻辑。

### 2. 容器一致性测试

分别基于 `std::unordered_map` 和 `std::map` 执行相同测试数据，对比两种实现生成的 WAL 和恢复结果是否一致。

### 3. 性能测试

使用固定随机种子生成百万级混合操作指令，统计总耗时和 QPS，用于比较不同底层容器和线程数下的性能表现。

在 2 核 CPU、4 GB 内存环境下，75% 读负载三轮测试结果显示：

```text
线程数    unordered_map 平均 QPS    map 平均 QPS
1        约 41.2 万                约 38.1 万
2        约 46.2 万                约 31.0 万
4        约 35.0 万                约 33.2 万
```

其中，`unordered_map` 在 2 个工作线程下表现最好，平均 QPS 约 46.2 万，较 `map` 方案提升约 49%。在 4 个工作线程下，`unordered_map` 仍高于 `map`，但提升收窄到约 5%。

### 4. 多线程测试

线程池并发提交随机请求，验证多线程执行稳定性、日志一致性和并发访问正确性。

### 5. WAL 恢复测试

使用已有 WAL 日志启动程序，验证恢复后的数据是否正确。恢复一致性测试建议使用单线程执行，再运行 `./kvt1`、`./kvt2` 和对比脚本。

### 6. 异常退出恢复测试

运行过程中使用：

```bash
kill -9 <pid>
```

模拟进程异常退出，重新启动后验证 WAL 是否可以恢复数据。

### 7. 恢复一致性校验

恢复完成后，对生成日志计算 SHA256：

```bash
sha256sum kv_test/test_1.txt kv_test/test_2.txt
```

用于验证两种底层容器的恢复结果是否一致。

---

## 项目设计

```text
客户端输入
   |
   v
  KV
   |
   +--> ThreadPool
          |
          v
        Log(WAL) ----> Entry
                         |
                         +--> map / unordered_map
```

---

## 后续优化方向

- Snapshot（快照）
- Batch Write（批量写日志）
- `fsync` 持久化策略
- LRU Cache
- 数据压缩
- TCP 网络接口
- 更细粒度的并发控制

