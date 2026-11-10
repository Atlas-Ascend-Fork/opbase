# IsIntegralType

## 功能说明

判断输入的数据类型是否为整数类型，包括DT_INT8、DT_INT16、DT_INT32、DT_INT64、DT_UINT8、DT_UINT16、DT_UINT32、DT_UINT64。

## 函数原型

```cpp
bool IsIntegralType(const ge::DataType type)
```

## 参数说明

| 参数 | 输入/输出 | 说明 |
| --- | --- | --- |
| type | 输入 | 输入的数据类型。 |

## 返回值说明

若为整数类型返回true，否则返回false。

## 约束说明

无

## 调用示例

```cpp
// 校验dtype是否为整数类型，不是则提前返回
void Func(const ge::DataType type) {
    if (!IsIntegralType(type)) {
        return;
    }
    // 后续执行算子计算逻辑
}
```
