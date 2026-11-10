# IsFloatingType

## 功能说明

判断输入的数据类型是否为浮点类型，包括DT_DOUBLE、DT_FLOAT、DT_BF16、DT_FLOAT16、DT_HIFLOAT8、DT_FLOAT8_E5M2、DT_FLOAT8_E4M3FN、DT_FLOAT8_E8M0、DT_FLOAT6_E3M2、DT_FLOAT6_E2M3、DT_FLOAT4_E2M1、DT_FLOAT4_E1M2、DT_HIFLOAT4、DT_HIFLOAT4_SCALE。

## 函数原型

```cpp
bool IsFloatingType(const ge::DataType type)
```

## 参数说明

| 参数 | 输入/输出 | 说明 |
| --- | --- | --- |
| type | 输入 | 输入的数据类型。 |

## 返回值说明

若为浮点类型返回true，否则返回false。

## 约束说明

无

## 调用示例

```cpp
// 校验dtype是否为浮点数类型，不是则提前返回
void Func(const ge::DataType dtype) {
    if (!IsFloatingType(dtype)) {
        return;
    }
    // 后续执行算子计算逻辑
}
```
