# AicpuTask Class

Manages parameters and methods related to AI CPU tasks, including task encapsulation, initialization, delivery, and execution.

The definition is as follows:

```cpp
class AicpuTask {
public:
    // opType is the operator type name; unknownType is the dynamic-shape processing category. Neither has a default.
    AicpuTask(const std::string &opType, const ge::UnknowShapeOpType unknownType)
        : opType_(opType), unknownType_(unknownType) {}
    virtual ~AicpuTask() = default;
    // Initializes task parameters and extended information: inputs/outputs are Tensor lists; attrs is the attribute map.
    // Implemented by derived classes. Returns ACLNN_SUCCESS on success or a specific error code on failure.
    virtual aclnnStatus Init(const FVector<const aclTensor *> &inputs, const FVector<aclTensor *> &outputs,
                             const AicpuAttrs &attrs) = 0;
    // Launches the task after SetIoTensors succeeds. executor must be valid; stream is the stream for this execution.
    // Returns ACLNN_SUCCESS or a specific error code. Success does not imply synchronized device completion on every path.
    virtual aclnnStatus Run(aclOpExecutor *executor, aclrtStream stream) = 0;
    // Refreshes inputs, outputs, shapes, and any existing workspace record from args before execution.
    // executor and args must be valid; args is borrowed. Returns ACLNN_SUCCESS or a specific error code.
    aclnnStatus SetIoTensors(aclOpExecutor *executor, op::OpArgContext *args);
    friend class AicpuTaskSpace;
    // space points to the owning AicpuTaskSpace and must remain valid while the task is used. No return value.
    void SetSpace(void *space)
    {
        space_ = space;
    }
    // visit is true to mark the task as occupied, or false to release it. Does not wait for device completion. Returns void.
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
    // The length may be insufficient and can be extended in the future.
    uint8_t inputKey_[kAicpuKeyBufLen] = {};
    size_t keyLen_ = 0;
    bool isVisit_ = false;
    uint64_t deviceExtMemSize_ = 0;
    uint64_t deviceCacheOffset_ = 0;
};
```

For details about the members in this class, see the following table.

**Table 1** AicpuTask class members

| Attribute| Type| Default Value| Description|
| --- | --- | --- | --- |
| opType_ | const std::string | "" | Name of the AI CPU operator corresponding to a task.|
| unknownType_ | const ge::UnknowShapeOpType | None (supplied by the constructor) | Dynamic-shape processing category of the operator. Values are defined by the `ge::UnknowShapeOpType` enumeration.|
| argsHandle_ | std::unique_ptr\<AicpuArgsHandler\> | null | Object for managing task encapsulation parameters.|
| extInfoHandle_ | std::unique_ptr\<AicpuExtInfoHandler\> | null | Object for managing extended task encapsulation parameters.|
| launchId_ | uint64_t | 0 | ID of a profiling task.|
| summaryItemId_ | uint64_t | 0 | ID of a summary item.|
| space_ | void* | null | Task management set to which a task belongs.|
| inputs_ | FVector<const aclTensor *> | null | List of input tensor pointers of the operator corresponding to a task.|
| outputs_ | FVector<const aclTensor *> | null | List of output tensor pointers of the operator corresponding to a task.|
| inputKey_ | uint8_t [kAicpuKeyBufLen] | 0 | Key field corresponding to a task.|
| keyLen_ | size_t | 0 | Length of the key field.|
| isVisit_ | bool | false | Whether the current task is in use.|
| deviceExtMemSize_ | uint64_t | 0 | Memory reserved on the device.|
| deviceCacheOffset_ | uint64_t | 0 | Offset of the reserved device memory.|
