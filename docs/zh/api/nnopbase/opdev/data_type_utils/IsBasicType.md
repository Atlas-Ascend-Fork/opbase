# IsBasicType

## 功能说明

判断输入的数据类型是否为基础类型，包括DT_COMPLEX128、DT_COMPLEX64、DT_DOUBLE、DT_FLOAT、DT_FLOAT16、DT_INT16、DT_INT32、DT_INT64、DT_INT8、DT_QINT16、DT_QINT32、DT_QINT8、DT_QUINT16、DT_QUINT8、DT_UINT16、DT_UINT32、DT_UINT64、DT_UINT8、DT_BF16。

## 函数原型

```cpp
bool IsBasicType(const DataType dtype)
```

## 参数说明

| 参数 | 输入/输出 | 说明 |
| --- | --- | --- |
| dtype | 输入 | 输入的数据类型。 |

## 返回值说明

输入数据类型为基础类型时，返回true，否则返回false。

## 约束说明

无

## 调用示例

```cpp
// 判断dtype不为基础类型时，返回
void Func(const DataType dtype) {
    if (!IsBasicType(dtype)) {
        return;
    }
}
```
