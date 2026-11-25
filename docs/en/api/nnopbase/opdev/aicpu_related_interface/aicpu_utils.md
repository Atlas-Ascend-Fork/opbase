# aicpu\_uitls

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** APIs

| API Definition| Description|
| --- | --- |
| RecordAicpuTime(const size_t index) | When timestamp recording is enabled and `index` is valid, locks `gTimeStampMutex` using `std::lock_guard<std::mutex>`, calls the system function `clock_gettime` to obtain a `CLOCK_MONOTONIC` timestamp, and writes it to `gAicpuTimeStamp.tp[index]`. The mutex is unlocked automatically when the scope ends. Recording is skipped if it is disabled or the index is out of range.|

## Parameters and Return Value

| Parameter or Object | Type | Meaning and Constraints |
| --- | --- | --- |
| `index` | `const size_t` | Required input parameter of `RecordAicpuTime`, with no default value. Selects the timestamp slot to write. The valid range is `0 <= index < kAicpuTimeStampNum`; the current array has 10 slots, indexed from 0 to 9. |
| `gTimeStampMutex` | `std::mutex` | Mutex protecting the timestamp recording code. |
| `gAicpuTimeStamp` | `AicpuTimeStamp` | Global timestamp structure containing the `timespec tp[kAicpuTimeStampNum]` array and the boolean flag `isEnable`. `tp[index]` is the selected slot; `tv_sec` and `tv_nsec` store seconds and nanoseconds, respectively. |

`RecordAicpuTime` returns `void` and has no return value.
