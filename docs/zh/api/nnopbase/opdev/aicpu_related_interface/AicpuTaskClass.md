# AicpuTask类

AicpuTask类用于管理AI CPU任务相关的一些参数和方法，包括组装任务、任务初始化、任务下发、任务执行等功能。

具体定义如下：

```cpp
class AicpuTask {
public:
    // opType 为算子类型名称，unknownType 为动态形状处理类别，均无默认值。
    AicpuTask(const std::string &opType, const ge::UnknowShapeOpType unknownType)
        : opType_(opType), unknownType_(unknownType) {}
    virtual ~AicpuTask() = default;
    // 初始化任务参数和扩展信息：inputs/outputs 为输入/输出 Tensor 列表，attrs 为属性映射。
    // 由派生类实现；成功返回 ACLNN_SUCCESS，失败返回具体错误码。
    virtual aclnnStatus Init(const FVector<const aclTensor *> &inputs, const FVector<aclTensor *> &outputs,
                             const AicpuAttrs &attrs) = 0;
    // 在 SetIoTensors 成功后下发任务；executor 为有效执行器，stream 为本次执行流。
    // 成功返回 ACLNN_SUCCESS，失败返回具体错误码；成功不表示所有路径均已同步完成设备计算。
    virtual aclnnStatus Run(aclOpExecutor *executor, aclrtStream stream) = 0;
    // 执行前从 args 刷新输入、输出、形状和已有 workspace 记录；executor、args 须有效。
    // 本方法借用 args；成功返回 ACLNN_SUCCESS，失败返回具体错误码。
    aclnnStatus SetIoTensors(aclOpExecutor *executor, op::OpArgContext *args);
    friend class AicpuTaskSpace;
    // space 为所属 AicpuTaskSpace 的地址，有效期覆盖任务使用周期；无返回值。
    void SetSpace(void *space)
    {
        space_ = space;
    }
    // visit 为 true 表示占用，为 false 表示释放占用；不等待设备任务完成，无返回值。
    void SetVisit(bool visit);
protected:
    const std::string opType_;
    const ge::UnknowShapeOpType unknownType_;
    std::unique_ptr<AicpuArgsHandler> argsHandle_;
    std::unique_ptr<AicpuExtInfoHandler> extInfoHandle_;
    uint64_t launchId_ = 0U;
    uint64_t summaryItemId_ = 0U;
    void *space_ = nullptr;
    FVector<const aclTensor *> inputs_;
    FVector<aclTensor *> outputs_;
    // 长度可能不够，后续考虑长度可扩展
    uint8_t inputKey_[kAicpuKeyBufLen] = {};
    size_t keyLen_ = 0;
    bool isVisit_ = false;
    uint64_t deviceExtMemSize_ = 0;
    uint64_t deviceCacheOffset_ = 0;
};
```

类成员属性的详细介绍请参考下表。

**表 1** AicpuTask类成员说明

| 属性名 | 属性类型 | 默认值 | 属性说明 |
| --- | --- | --- | --- |
| opType_ | const std::string | "" | Task对应的AI CPU算子名。 |
| unknownType_ | const ge::UnknowShapeOpType | 无（构造函数传入） | 算子的动态形状处理类别，取值见ge::UnknowShapeOpType枚举。 |
| argsHandle_ | std::unique_ptr\<AicpuArgsHandler\> | null | 管理task封装参数的对象。 |
| extInfoHandle_ | std::unique_ptr\<AicpuExtInfoHandler\> | null | 管理task封装拓展参数的对象。 |
| launchId_ | uint64_t | 0 | 用于profiling采集的任务下发id。 |
| summaryItemId_ | uint64_t | 0 | 用于统计条目的id。 |
| space_ | void* | null | 记录task属于哪个task管理集合。 |
| inputs_ | FVector<const aclTensor *> | null | task对应算子的输入tensor指针列表。 |
| outputs_ | FVector<const aclTensor *> | null | task对应算子的输出tensor指针列表。 |
| inputKey_ | uint8_t [kAicpuKeyBufLen] | 0 | task对应的key字段。 |
| keyLen_ | size_t | 0 | key字段长度。 |
| isVisit_ | bool | false | 当前task是否正被使用。 |
| deviceExtMemSize_ | uint64_t | 0 | device侧预留的内存。 |
| deviceCacheOffset_ | uint64_t | 0 | device侧预留内存的偏移。 |
