# aicpu\_utils

本章接口为预留接口，后续有可能变更或废弃，不建议开发者使用，开发者无需关注。

**表 1** 接口列表

| 接口定义 | 功能说明 |
| --- | --- |
| RecordAicpuTime(const size_t index) | 在时间戳记录已启用且 `index` 有效时，通过 `std::lock_guard<std::mutex>` 锁定 `gTimeStampMutex`，调用系统函数 `clock_gettime` 获取 `CLOCK_MONOTONIC` 单调时钟时间戳，并写入 `gAicpuTimeStamp.tp[index]`；离开作用域时自动解锁。未启用记录或索引越界时跳过记录。 |

## 参数与返回值说明

| 参数或对象 | 类型 | 含义与约束 |
| --- | --- | --- |
| `index` | `const size_t` | `RecordAicpuTime` 的输入参数，必填且无默认值，表示写入的时间戳槽位。有效范围为 `0 <= index < kAicpuTimeStampNum`，当前长度为 10，即 0～9。 |
| `gTimeStampMutex` | `std::mutex` | 保护时间戳记录代码段的互斥锁对象。 |
| `gAicpuTimeStamp` | `AicpuTimeStamp` | 全局时间戳结构实例，包含 `timespec tp[kAicpuTimeStampNum]` 数组和布尔开关 `isEnable`；`tp[index]` 为对应槽位，`tv_sec`、`tv_nsec` 分别表示秒和纳秒字段。 |

`RecordAicpuTime` 返回 `void`，无返回值。
