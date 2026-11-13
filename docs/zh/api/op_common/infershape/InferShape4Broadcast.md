# InferShape4Broadcast

## 功能说明

图模式场景下，broadcast类算子的InferShape方法（用于推导输出张量的形状）。不带inputNum的重载固定使用前两个输入；另一重载使用inputNum指定输入个数。

## 函数原型

```cpp
ge::graphStatus InferShape4Broadcast(gert::InferShapeContext* context)
```

```cpp
ge::graphStatus InferShape4Broadcast(gert::InferShapeContext* context, size_t inputNum)
```

## 参数说明

| 参数名 | 输入/输出 | 说明 |
| --- | --- | --- |
| context | 输入 | GE传入的InferShape上下文。 |
| inputNum | 输入 | 参与broadcast的输入个数，需大于等于1。 |

## 返回值说明

返回类型为ge::graphStatus：

- ge::GRAPH\_SUCCESS：InferShape成功。
- ge::GRAPH\_FAIL：InferShape失败。

## 约束说明

无

## 调用示例

关键代码示例如下，仅供参考，不支持直接拷贝运行。

```cpp
// 双输入场景：Add算子按broadcast规则推导输出shape
ge::graphStatus InferShape4Add(gert::InferShapeContext* context)
{
    return Ops::Base::InferShape4Broadcast(context);
}
IMPL_OP_INFERSHAPE(Add).InferShape(InferShape4Add);

// 多输入场景：多输入broadcast类算子
ge::graphStatus InferShape4MyOp(gert::InferShapeContext* context)
{
    return Ops::Base::InferShape4Broadcast(context, 3); // 3个输入
}
IMPL_OP_INFERSHAPE(MyOp).InferShape(InferShape4MyOp);
```
