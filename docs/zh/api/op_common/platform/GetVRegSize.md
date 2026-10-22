# GetVRegSize

## 功能说明

获取平台向量寄存器（Vector Register）的大小，单位bytes。

## 函数原型

```cpp
template <typename T>
uint32_t GetVRegSize(const T *context)
```

## 参数说明

| 参数名 | 输入/输出 | 说明 |
| --- | --- | --- |
| context | 输入 | Tiling的上下文信息，类型为TilingContext/TilingParseContext。 |

## 返回值说明

uint32\_t：返回当前平台的向量寄存器大小，单位bytes。取值随平台而异，平台信息或接口获取失败时默认返回256bytes。

## 约束说明

无

## 调用示例

关键代码示例如下，仅供参考，不支持直接拷贝运行。

```cpp
compileInfo->vRegSize = GetVRegSize(context_);
```
