# aicpu\_args\_handler

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** APIs

| API Definition| Description|
| --- | --- |
| AicpuArgsHandler(const std::string &opType, const std::string &nodeName, const uint32_t ioNum, const bool needDeviceExt) | Indicates the auxiliary class used by the AI CPU framework to construct AI CPU opTask.|
| opType_(opType) | Indicates the operator name.|
| nodeName_(nodeName) | Indicates the node name, which is the same as the operator name. This is a reserved parameter.|
| ioNum_(ioNum) | Indicates the total number of inputs and outputs of an operator.|
| needDeviceExt_(needDeviceExt) | Indicates whether additional device memory needs to be allocated.|
| args_({}) | Indicates the object of the parameters for interacting with Runtime. It is used to deliver computing tasks.|
| GetIoAddr() | Obtains the pointer to memory for storing the input/output address in the args memory.|
| get() | Obtains the start address of memory managed by the smart pointer.|
| GetExtInfoAddr() | Obtains the pointer to memory for storing extended parameters in args.|
| GetArgs() | Obtains the pointer to the args memory.|
| GetArgsEx() | Obtains the pointer to the object for interacting with Runtime.|
| GetNodeName() | Obtains the node name, which is the same as the operator name.|
| GetIoNum() | Obtains the total number of inputs and outputs of an operator.|
| GetHostInputSize() | Obtains the input size of host memory.|
| GetInputAddrAlignBytes() | Obtains the byte alignment of specified address allocation.|
| SetSpace(void *space) | Sets the operator space to which the args handler belongs.|
| MallocMem() | Allocates memory for args.|
| ResetHostInputInfo() | Resets the input information on the host.|
| AddHostInput(const size_t idx, void *data, const size_t srcSize, const size_t alignSize) | Records the input information on the host.|
| UpdateIoAddr(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const aclrtStream stream, aclOpExecutor *executor, const uint64_t deviceExtMemSize, const uint64_t deviceCacheOffset) | Updates the input and output addresses in args.|
| AicpuArgsHandler() | Indicates the auxiliary class used by the AI CPU framework to construct AI CPU opTask.|
| UpdateDeviceExtInfoAddr(void *deviceExtInfoAddr) | Updates the address for storing extended parameters on the device.|
| SetLaunchArgs(const size_t argSize) | Sets the sizes of parameters in args.|
| GetDeviceCacheAddr(void *&deviceAddr, aclOpExecutor *executor, const uint64_t deviceCacheOffset) | Obtains the address of reserved memory on the device.|
| AicpuCCArgsHandler(const std::string &opType, const std::string &nodeName, const uint32_t ioNum, const bool needDeviceExt) | Indicates the class for managing AI CPU CANN framework parameters.|
| GenCCArgs(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const AicpuAttrs &attrs, std::string &taskInfo) | Generates the AicpuParamHead, I/O address, and nodedef information in args of the CANN operator.|
| BuildCCArgs(const std::string &argData, const std::string &kernelName, const std::string &soName, const size_t extInfoSize) | Encapsulates parameters in args of the CANN operator.|
| SetHostArgs(const std::string &argData, const size_t extInfoSize) | Sets the address and length of memory in args.|
| SetOffsetArgs() | Sets the offset value in args.|
| AicpuTfArgsHandler(const std::string &opType, const std::string &nodeName, const uint32_t ioNum, const bool needDeviceExt) | Indicates the class for managing AI CPU TensorFlow framework parameters.|
| GenTfArgs(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const AicpuAttrs &attrs, STR_FWK_OP_KERNEL &fwkOpKernel, std::string &taskInfo) | Generates the KernelRunParam and nodedef information in args of the TensorFlow operator, and fills `STR_FWK_OP_KERNEL`.|
| BuildTfArgs(STR_FWK_OP_KERNEL &fwkOpKernel, const std::string &taskInfo, const size_t extInfoSize) | Encapsulates parameters in args of the TensorFlow operator.|
| GenNodeDef(const FVector<const aclTensor *> &inputs, const AicpuAttrs &attrs, ge::Buffer &nodeDefBytes) | Generates the operator description, including the input and output addresses and shapes.|
| UpdateKernelId() | Updates the Kernel field in args to match the kernel cache on the device.|

## Parameters

### Constructor Parameters

The three classes use constructor parameters with the same meaning. All are required inputs with no default values.

| Name | Type | Required | Value Range | Unit | Constraints |
| --- | --- | --- | --- | --- | --- |
| `opType` | `const std::string &` | Yes | Type name matching the target operator. | None | Operator type name. The constructor does not check whether the string is empty. |
| `nodeName` | `const std::string &` | Yes | Node name string. | None | The current task initialization paths pass the same value as `opType`; the constructor does not check their equality. |
| `ioNum` | `const uint32_t` | Yes | 0 to UINT32_MAX. | Count | Total number of input and output address slots. Must match the subsequent input/output lists and serialized parameters. |
| `needDeviceExt` | `const bool` | Yes | `true` or `false`. | None | Whether separate device memory is used for extended information. The current task paths pass `true` for `ge::DEPEND_SHAPE_RANGE` and `false` otherwise. |

### Host Input Parameters

All `AddHostInput` parameters are required inputs with no default values.

| Name | Type | Required | Value Range | Unit | Constraints |
| --- | --- | --- | --- | --- | --- |
| `idx` | `const size_t` | Yes | Valid index in the current input list, starting at 0 and less than `ioNum_`. | None | Input address slot index. The caller must ensure it is valid; the method does not check the index range. |
| `data` | `void *` | Yes | Valid source address when `srcSize > 0`; may be null when `srcSize == 0`. | None | Host source data address with at least `srcSize` readable bytes. Copying is skipped when the size is 0. |
| `srcSize` | `const size_t` | Yes | 0 to `alignSize`. | Bytes | Actual number of source bytes to copy. Must match the source data. |
| `alignSize` | `const size_t` | Yes | At least `srcSize`; the accumulated aligned size must not exceed 1024. | Bytes | Data size rounded up to the alignment granularity. The current caller uses `RoundUp(srcSize, GetInputAddrAlignBytes())`; this value is the aligned size, not the alignment granularity. |

The buffer must be allocated and its offsets set before the call. The caller must ensure the index, source data, and size relationships are valid. The accumulated aligned size must satisfy `hostInputSize_ + alignSize <= 1024`, in bytes. `GetInputAddrAlignBytes()` provides the alignment granularity: initially 4 bytes on the CANN path and 64 bytes after TensorFlow parameters are built.

### Address Update and Task Space Parameters

All parameters below must be supplied explicitly and have no default values. Sizes and offsets are measured in bytes.

| Name | Type | Required | Value Range | Unit | Constraints |
| --- | --- | --- | --- | --- | --- |
| `SetSpace.space` | `void *` | Yes | Valid task space address. | None | Input address of the owning `AicpuTaskSpace`, which must remain valid while the task is used. TensorFlow parameter generation reads reference indices through this address; a valid space must be bound first. |
| `UpdateIoAddr.inputs` | `const FVector<const aclTensor *> &` | Yes | Valid input list; null input slots are allowed. | None | Inputs for this execution, in the order used when the parameters were built. Null inputs produce address 0. Host inputs use the copy or device cache path. |
| `UpdateIoAddr.outputs` | `const FVector<aclTensor *> &` | Yes | Output list with valid elements placed on `kOnDeviceHbm`. | None | Outputs for this execution, in the order used when the parameters were built. A non-device output causes an internal error. |
| `UpdateIoAddr.stream` | `const aclrtStream` | Yes | Valid runtime stream for this execution. | None | Stream used for asynchronous copies. Must match the executor stream and the subsequent task launch stream. |
| `UpdateIoAddr.executor` | `aclOpExecutor *` | Yes | Valid non-null executor pointer. | None | Used to obtain the workspace base address and device cache budget. |
| `UpdateIoAddr.deviceExtMemSize` | `const uint64_t` | Yes | 0 to the executor device AI CPU memory budget. | Bytes | Reserved size of device extended information. |
| `UpdateIoAddr.deviceCacheOffset` | `const uint64_t` | Yes | Valid offset within the workspace region reserved for the device cache. | Bytes | Offset of the device cache for host inputs relative to the workspace base address. Determined when extended information is refreshed before execution. |
| `GetDeviceCacheAddr.deviceAddr` | `void *&` | Yes | Valid device cache address written by the method. | None | Output parameter receiving the device cache address available for this operation. |
| `GetDeviceCacheAddr.executor` | `aclOpExecutor *` | Yes | Valid non-null executor pointer. | None | The workspace must already be allocated, with sufficient reserved space for the data. |
| `GetDeviceCacheAddr.deviceCacheOffset` | `const uint64_t` | Yes | Valid offset within the workspace region reserved for the device cache. | Bytes | Workspace offset used when the device cache base address is first established. The returned address also includes the used size `deviceCacheSize_`. |
| `UpdateDeviceExtInfoAddr.deviceExtInfoAddr` | `void *` | Yes | Valid address when separate device extended information is required; may be null on other paths. | None | Input device extended information address. Other paths use runtime placeholder records. |

### Parameter Generation and Construction Parameters

All parameters below must be supplied explicitly and have no default values.

| Name | Type | Required | Value Range | Unit | Constraints |
| --- | --- | --- | --- | --- | --- |
| `GenCCArgs/GenTfArgs.inputs` | `const FVector<const aclTensor *> &` | Yes | Input Tensor list matching the target operator definition. | None | Used to generate parameter descriptions. The order must match subsequent execution. |
| `GenCCArgs/GenTfArgs.outputs` | `const FVector<aclTensor *> &` | Yes | Output Tensor list matching the target operator definition. | None | Used to generate parameter descriptions. The order must match subsequent execution. |
| `GenCCArgs/GenTfArgs.attrs` | `const AicpuAttrs &` | Yes | Attribute map matching the operator definition; may be empty. | None | Input attribute map. Use an empty map if there are no attributes. |
| `GenCCArgs/GenTfArgs.taskInfo` | `std::string &` | Yes | String to which binary task parameters can be appended. | None | Input/output parameter receiving the generated binary task parameters. The current construction paths pass an empty string; the methods do not clear existing content first. |
| `GenTfArgs.fwkOpKernel` | `STR_FWK_OP_KERNEL &` | Yes | Writable TensorFlow framework launch structure. | None | Input/output parameter filled for subsequent use by `BuildTfArgs`. |
| `BuildCCArgs/SetHostArgs.argData` | `const std::string &` | Yes | Binary parameters produced by the CANN generation path. | None | Input parameter. The I/O count in its parameter header must match `ioNum_`. |
| `BuildCCArgs.kernelName` | `const std::string &` | Yes | Function name provided by operator loading or registration. | None | Input name of the kernel function to launch. |
| `BuildCCArgs.soName` | `const std::string &` | Yes | Shared library name provided by operator loading. | None | Input shared library name. The implementation retains special handling for custom operator libraries. |
| `BuildCCArgs/SetHostArgs/BuildTfArgs.extInfoSize` | `const size_t` | Yes | Actual serialized length of extended information. | Bytes | Input parameter that must match the actual extended information length. |
| `BuildTfArgs.fwkOpKernel` | `STR_FWK_OP_KERNEL &` | Yes | Framework launch structure filled by `GenTfArgs`. | None | Input/output parameter whose extended information length, kernel ID, and other fields are updated before it is copied into the buffer. |
| `BuildTfArgs.taskInfo` | `const std::string &` | Yes | Binary task parameters generated by `GenTfArgs`. | None | Input task parameters used to build the TensorFlow parameter buffer. |
