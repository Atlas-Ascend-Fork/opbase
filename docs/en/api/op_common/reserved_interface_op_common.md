# Reserved APIs

This document summarizes the list of APIs that are not externally committed, describing the name and function of each API. These APIs may be changed or deprecated in the future. Developers are advised not to use them and do not need to pay attention to them.

**Table 1** API List

| API Name | Description |
| --- | --- |
| GetTid() | Obtains the current thread ID. |
| GetSafeStr(T name) | Safely obtains a string pointer, supporting const char*, char*, and std::string types. |
| GetOpInfo(const std::string& str) | Obtains the operator information string. |
| GetOpInfo(const char* str) | Obtains the operator information string, with null pointer protection. |
| IsContextType<T\>() | Determines whether the template type T is a Context type. |
| GetOpInfo(T context) | Obtains the operator type and name information from TilingContext or InferShapeContext. |
| DLOG_DEBUG | DEBUG-level log constant, with a value of 0. |
| DLOG_INFO | INFO-level log constant, with a value of 1. |
| DLOG_WARN | WARN-level log constant, with a value of 2. |
| DLOG_ERROR | ERROR-level log constant, with a value of 3. |
| OP_MODULE_ID | Operator module ID constant, with a value of 63. |
| CheckLogLevel(int32_t moduleId, int32_t logLevel) | Log level check function that determines whether the log level of a specified module is enabled. It returns 1 if enabled. |
| DlogRecord(int32_t moduleId, int32_t level, const char* fmt, ...) | Log recording function that records logs by module ID and level. |
| OP_LOGE_LIBOPAPI_REPORT(opName, fmt, ...) | Underlying log reporting macro that records ERROR-level logs. |
| OP_LOGE_WITHOUT_REPORT(opName, ...) | Log macro that only records logs without reporting errors. |
| OpLogSub(moduleId, level, opInfo, fmt, ...) | Log recording macro of a specified module and level. |
| OpLogErrSub(moduleId, level, opInfo, fmt, ...) | Error log recording macro of a specified module and level. |
| D_OP_LOGI(opName, fmt, ...) | INFO-level log debugging macro. |
| D_OP_LOGW(opName, fmt, ...) | WARN-level log debugging macro. |
| D_OP_LOGE(opName, fmt, ...) | ERROR-level log debugging macro. |
| D_OP_LOGD(opName, fmt, ...) | DEBUG-level log debugging macro. |
| unlikely(x) | Compiler branch prediction optimization macro, marking x as a low-probability branch. |
| likely(x) | Compiler branch prediction optimization macro, marking x as a high-probability branch. |
| ToString(ge::DataType type) | Obtains the string representation corresponding to DataType. |
| ToString(ge::Format format) | Obtains the string representation corresponding to Format. |
| ToString(const gert::Shape& shape) | Obtains the string representation corresponding to Shape. |
| ToString(const std::vector<const gert::Shape*\>& v) | Obtains the string representation corresponding to a list of Shape pointers. |
| ViewErrorCode | Error code enum used for error reporting in scenarios such as InferShape. |
| LastPow2(uint64_t n) | Obtains the maximum power of 2 not greater than n. |
| MurmurHash(const void *src, uint32_t srcLen, uint32_t seed) | Murmur hash algorithm, used to compute the hash value of data. |
| SplitResult | Even split result struct, including splitCount, splitFactor, and splitTailFactor. |
| SplitIntoEqualByParts(int64_t splitLen, int32_t parts, SplitResult& splitResult) | Evenly splits splitLen into parts shares and returns the size of each share. |
| SplitIntoEqualByFactor(int64_t splitLen, int32_t factor, SplitResult& splitResult) | Evenly splits splitLen by the specified factor and returns the number of shares. |
| GetValueToInteger<T1, T2\>(const gert::Tensor *constTensor, T2 &value) | Reads integer-type data from a const Tensor. |
| GetConstInt<ContextType, T\>(const ContextType *context, int64_t inputIdx, T &value) | Obtains the constant integer value of a specified input from InferShapeContext or TilingContext. |
| GetValueToShape<T\>(const gert::Tensor *constTensor, gert::Shape &shape) | Reads an integer array from a const Tensor into a Shape. |
| GetConstIntToShape<ContextType\>(const ContextType *context, int64_t inputIdx, gert::Shape &shape) | Obtains the constant integer of a specified input from InferShapeContext or TilingContext and writes it into a Shape. |
| IsRegbaseSocVersion(const gert::TilingParseContext* context) | Determines whether the current platform is a Regbase SoC version. |
| IsRegbaseSocVersion(const gert::TilingContext* context) | Determines whether the current platform is a Regbase SoC version. |
| EnsureNotScalar(const gert::Shape& inShape) | Ensures that the Shape is not a scalar. If it is a scalar, returns a one-dimensional Shape. |
| AiCoreParams | AI Core parameter struct, including ubSize, numBlocks, aicNum, l1Size, l0aSize, l0bSize, and l0cSize. |
| CompileInfoCommon | Compilation information common struct, including platform information such as aivNum, aicNum, ubSize, and l1Size. |
| TilingBaseClass(gert::TilingContext* context) | Tiling base class constructor that receives TilingContext. |
| DoTiling() | Tiling execution framework entry that invokes each Tiling phase in turn. |
| Reset(gert::TilingContext* context) | Resets the context member of TilingBaseClass. |
| IsCapable() | Pure virtual function that determines whether the current class supports the Tiling of the operator. |
| GetPlatformInfo() | Pure virtual function that obtains platform information such as CoreNum and the UB/L1/L0C resource sizes. |
| GetShapeAttrsInfo() | Pure virtual function that obtains INPUT/OUTPUT/ATTR information. |
| DoOpTiling() | Pure virtual function that computes the data segmentation TilingData. |
| DoLibApiTiling() | Pure virtual function that computes the TilingData of the high-level API. |
| GetTilingKey() | Pure virtual function that computes TilingKey. |
| GetWorkspaceSize() | Pure virtual function that computes the Workspace size. |
| PostTiling() | Pure virtual function that saves Tiling data. |
| DumpTilingInfo() | Prints Tiling data information. |
| DefaultTilingInfoDump() | Default Tiling data printing implementation. |
| CalcTschBlockDim(uint32_t sliceNum, uint32_t aicCoreNum, uint32_t aivCoreNum) | Computes BlockDim in tsch scenarios. |
| GetShapeDebugStr<T\>(const T& shape) | Obtains the debug string of a Shape. |
| GetTensorDebugStr(const gert::StorageShape*shape, const gert::CompileTimeTensorDesc* tensor) | Obtains the debug string of a Tensor, including dtype, shape, and format information. |
| GetTilingContextDebugStr() | Obtains the debug string of all input and output Tensors in TilingContext. |
| GetTilingDataDebugStr() | Obtains the debug string of TilingData. |
| bfloat16 | bfloat16 data type struct, supporting conversion with types such as float and double and arithmetic operations. |
| fp16_t | fp16 half-precision floating point data type struct, supporting conversion with types such as float and int and arithmetic operations. |
| castTraitB162B32 | CastTrait constant configuration from B16 to B32. |
| castTraitB322B16 | CastTrait constant configuration from B32 to B16. |
| castTraitB322Int32 | CastTrait constant configuration from B32 to Int32. |
| castTraitB322Int16 | CastTrait constant configuration from B32 to Int16. |
| castTraitB162Int8 | CastTrait constant configuration from B16 to Int8. |
| LoadOneTensorForDtypeT<T\>(__local_mem__ T *input, MicroAPI::RegTensor<float\> &dst, MicroAPI::MaskReg &preg, uint32_t offset) | Loads an aligned Tensor from UB into a float32 register. |
| LoadTwoTensorForDtypeT<T\>(__local_mem__ T *src1, __local_mem__ T*src2, MicroAPI::RegTensor<float\> &dst1, MicroAPI::RegTensor<float\> &dst2, MicroAPI::MaskReg &dst1Preg, MicroAPI::MaskReg &dst2Preg, uint32_t src1Offset, uint32_t src2Offset) | Loads two aligned Tensors from UB into float32 registers. |
| StoreOneTensorForDtypeT<T\>(__local_mem__ T *output, MicroAPI::RegTensor<float\> &src, MicroAPI::MaskReg &preg, uint32_t offset) | Stores float32 register data into UB and supports bfloat16/float16/float32/int32/int16/int8/uint8 type outputs. |
| LoadUnAlignOneTensor<T\>(__local_mem__ T *&input, MicroAPI::RegTensor<float\> &dst, MicroAPI::UnalignReg &uSrc, MicroAPI::MaskReg &preg, uint32_t postUpdateStride) | Loads an unaligned Tensor from UB into a float32 register. |
| StoreUnAlignOneTensor<T\>(__local_mem__ T *&output, MicroAPI::RegTensor<float\> &src, MicroAPI::UnalignReg &uValue, MicroAPI::MaskReg &preg, uint32_t postUpdateStride) | Stores float32 register data into UB in an unaligned manner. |
| CeilDiv<T\>(T a, T b) | Kernel-side division rounded up. |
| FloorDiv<T\>(T a, T b) | Kernel-side division rounded down. |
| CeilAlign<T\>(T a, T b) | Kernel-side upward alignment. |
| FloorAlign<T\>(T a, T b) | Kernel-side downward alignment. |
| GetUbBlockSize() | Obtains the UB block unit size on the kernel side (32 bytes). |
| GetVRegSize() | Obtains the vector register size on the kernel side. |
| aclOpExecutor | acl operator executor struct type. |
| aclTensor | acl tensor struct type. |
| aclScalar | acl scalar struct type. |
| aclTensorList | acl tensor list struct type. |
| _aclCreateTensor | Function pointer type of the aclCreateTensor API. |
| _aclCreateScalar | Function pointer type of the aclCreateScalar API. |
| _aclCreateIntArray | Function pointer type of the aclCreateIntArray API. |
| _aclCreateFloatArray | Function pointer type of the aclCreateFloatArray API. |
| _aclCreateBoolArray | Function pointer type of the aclCreateBoolArray API. |
| _aclCreateTensorList | Function pointer type of the aclCreateTensorList API. |
| _aclDestroyTensor | Function pointer type of the aclDestroyTensor API. |
| _aclDestroyScalar | Function pointer type of the aclDestroyScalar API. |
| _aclDestroyIntArray | Function pointer type of the aclDestroyIntArray API. |
| _aclDestroyFloatArray | Function pointer type of the aclDestroyFloatArray API. |
| _aclDestroyBoolArray | Function pointer type of the aclDestroyBoolArray API. |
| _aclDestroyTensorList | Function pointer type of the aclDestroyTensorList API. |
| ResetCacheThreadLocal | Function pointer type of the ResetCacheThreadLocal API. |
| GET_OP_API_FUNC(apiName) | Obtains the function address of the specified acl operator API and converts it into the corresponding function pointer type. |
| index_sequence<Is...\> | Compile-time index sequence struct template. |
| make_index_sequence_helper<N, Is...\> | Compile-time index sequence generation helper struct template. |
| make_index_sequence<N\> | Alias template for generating a compile-time index sequence. |
| GetOpApiLibName() | Obtains the built-in opapi operator library name. |
| GetCustOpApiLibName() | Obtains the custom operator opapi library name. |
| GetOpApiFuncAddrInLib(void *handler, const char*libName, const char *apiName) | Finds the function address by symbol name in a specified dynamic library. |
| GetOpApiLibHandler(const char *libName) | Loads a specified dynamic library and returns the handle. |
| GetAclnnAddrByApiName(const char *apiName) | Traverses the opapi domain library to find the function address. |
| GetOpApiFuncAddr(const char *apiName) | Finds the function address from the cust_opapi library, opapi library, and domain library in sequence. |
| GetConvertType(const gert::Tensor *ge_tensor) | Converts a GE data type into an acl data type. |
| ConvertType(const gert::Tensor *ge_tensor) | Converts a GE Tensor into an acl Tensor. |
| ConvertType(std::vector<const gert::Tensor *\>& ge_tensorList) | Converts a GE Tensor list into an acl TensorList. |
| ConvertType(T value) | Generic type conversion template. Non-Tensor types are returned as-is. |
| ConvertScalarType(T value) | Converts a scalar into an acl Scalar. |
| Release(aclTensor *p) | Releases an acl Tensor. |
| Release(aclScalar *p) | Releases an acl Scalar. |
| Release(aclTensorList *p) | Releases an acl TensorList. |
| Release(T value) | Generic type release template. Non-acl types are not processed. |
| CallRelease(Tuple t, index_sequence) | Expands the tuple and calls Release one by one to release resources. |
| ReleaseConvertTypes(Tuple &t) | Releases all conversion results in the tuple. |
| ConvertTypes(Ts&... args) | Converts multiple GE-type parameters into acl types and packages them into a tuple. |
| call(Function f, Tuple t, index_sequence) | Expands the tuple parameters and invokes the target function. |
| call(Function f, Tuple t) | Expands the tuple parameters and invokes the target function. |
| ConvertToOpApiFunc(params, opApiAddr, index_sequence) | Converts the function address into a function pointer matching the parameter type. |
| ConvertToOpApiFunc(params, opApiAddr) | Converts the function address into a function pointer matching the parameter type. |
| OpApiAnyValueDeleter | Generic parameter resource destruction function pointer type. |
| OpApiAnyValue | Generic parameter container struct that stores the parameter pointer and its destructor. |
| OpApiFunc2Stage | Two-phase Launch function pointer type. |
| OpApiParams | Two-phase execution parameter struct that encapsulates the converted parameter list, operator executor, and Launch function. |
| Collect(aclTensor *p, std::vector<OpApiAnyValue\>& params) | Collects an acl Tensor parameter and its destructor into the parameter list. |
| Collect(aclScalar *p, std::vector<OpApiAnyValue\>& params) | Collects an acl Scalar parameter and its destructor into the parameter list. |
| Collect(aclIntArray *p, std::vector<OpApiAnyValue\>& params) | Collects an acl IntArray parameter and its destructor into the parameter list. |
| Collect(aclBoolArray *p, std::vector<OpApiAnyValue\>& params) | Collects an acl BoolArray parameter and its destructor into the parameter list. |
| Collect(aclTensorList *p, std::vector<OpApiAnyValue\>& params) | Collects an acl TensorList parameter and its destructor into the parameter list. |
| Collect(T value, std::vector<OpApiAnyValue\>& params) | Generic parameter collection template. Non-acl types are not processed. |
| CallCollect(Tuple t, index_sequence, std::vector<OpApiAnyValue\>& params) | Expands the tuple and calls Collect one by one to collect parameters. |
| CollectConvertedTypes(Tuple &t, std::vector<OpApiAnyValue\>& params) | Collects all converted parameters and their destructors in the tuple. |
