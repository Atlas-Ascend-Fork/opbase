# aicpu\_ext\_info\_handle

本章接口为预留接口，后续有可能变更或废弃，不建议开发者使用，开发者无需关注。

**表 1** 接口列表

| 接口定义 | 功能说明 |
| --- | --- |
| AicpuExtInfoHandler(const std::string &nodeName, const uint32_t inputNum, const uint32_t outputNum, const ge::UnknowShapeOpType unknownType) | AI CPU拓展参数管理类构造函数。 |
| nodeName_(nodeName) | 算子名称。 |
| inputNum_(inputNum) | 算子的输入个数。 |
| outputNum_(outputNum) | 算子的输出个数。 |
| unknownType_(unknownType) | 标识算子的动态形状处理类别，取值见ge::UnknowShapeOpType枚举。 |
| AicpuExtInfoHandler() | AI CPU拓展参数管理类的默认构造函数，用于创建未指定算子的空实例。 |
| GenTfExtBuffer(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, std::string &taskExtInfo) | 生成TensorFlow算子框架的拓展参数。 |
| GenCCExtBuffer(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, std::string &taskExtInfo) | 生成CANN算子框架的拓展参数。 |
| Parse(const std::string &extInfo, uint8_t *hostAddr) | 解析拓展参数的函数。 |
| UpdateInputShape(const uint32_t inputIndex, const gert::Shape &inputShape) | 更新拓展参数中的输入shape信息。 |
| UpdateOutputShape(const uint32_t outputIndex, const gert::Shape &outputShape) | 更新拓展参数中的输出shape信息。 |
| GetOutputShapeAndType(const uint32_t outputIndex, gert::Shape &shape, ge::DataType &dataType) | 获取拓展参数中的shape和数据类型等信息。 |
| CopyH2D(const aclrtStream stream, const aclOpExecutor *executor, const uint64_t deviceExtMemSize, uint64_t &deviceCacheOffset) | 将拓展参数从host侧拷贝至device侧。 |
| CopyOutputShapeD2H() | 将device侧算子执行更新后的输出shape更新到host侧。 |
| GetExtInfoDeviceBuffer(const aclOpExecutor *executor, const uint64_t deviceExtMemSize, uint64_t &deviceCacheOffset) | 获取device侧预留的拓展参数内存地址。 |
| GenerateKernelId() | 生成kernel id。 |
| UpdateOutputShapeFromExtInfo(const FVector<aclTensor *> &outputs, aclrtStream stream) | 从device侧获取输出shape信息更新到host侧。 |
| UpdateInputAndOutputShape(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, aclrtStream stream, const aclOpExecutor *executor, const uint64_t deviceExtMemSize, uint64_t &deviceCacheOffset) | 二阶段执行前，刷新真实的输入、输出shape信息更新到device侧。 |
| bool HasWorkspaceInfo() const | 查询Parse解析的拓展参数中是否包含workspace记录。返回true表示记录存在，返回false表示不存在；记录存在不代表workspace地址已经更新。 |
| aclnnStatus UpdateWorkspaceInfo(uint64_t size, uint64_t addr) | 更新host侧已有workspace记录。size为当前算子所需的原始字节数且大于0，addr为该workspace的实际设备地址且非0。<br>成功返回ACLNN_SUCCESS；无记录、size为0或addr为0时返回错误码。调用方负责保证地址和长度有效，更新后由现有参数下发流程传至device。 |
| SetSpace(void *space) | 设置使用的task空间。 |
| ParseExtShapeType(AicpuExtInfo &aicpuExtInfo) | 解析拓展参数中的shape和数据类型信息。 |
| ParseExtInputShape(AicpuExtInfo &aicpuExtInfo) | 解析拓展参数中的输入shape。 |
| ParseExtOutputShape(AicpuExtInfo &aicpuExtInfo) | 解析拓展参数中的输出shape。 |
| AppendExtOpName(std::string &taskExtInfo) | 向拓展参数中拼接算子名称。 |
| AppendExtShapeType(std::string &taskExtInfo) | 向拓展参数中拼接shape和类型信息。 |
| AppendExtBitMap(std::string &taskExtInfo) | 向拓展参数中拼接bitmap信息。 |
| AppendExtInfoShape(const FVector<const aclTensor *> &tensors, const aicpu::FWKAdapter::FWKTaskExtInfoType type, std::string &taskExtInfo, bool isTf = false) | 向拓展参数中拼接shape信息。 |
| AppendSessionInfo(std::string &taskExtInfo) | 向拓展参数中拼接session信息。 |
| UpdateShape(const gert::Shape &shape, AicpuShapeAndType *const shapeAndType) | 更新shape信息。 |
| GetShapeAndType(const AicpuShapeAndType &shapeAndType, gert::Shape &shape, ge::DataType &dataType) | 获取拓展参数中的shape和数据类型信息。 |
