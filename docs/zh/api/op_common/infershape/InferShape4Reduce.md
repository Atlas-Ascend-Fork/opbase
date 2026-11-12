# InferShape4Reduce

## 功能说明

图模式场景下，reduce类算子的InferShape方法（用于推导输出张量的形状shape）。

## 函数原型

```cpp
ge::graphStatus InferShape4Reduce(gert::InferShapeContext *context)
```

## 参数说明

| 参数名 | 输入/输出 | 说明 |
| --- | --- | --- |
| context | 输入 | GE传入的InferShape上下文。 |

## 返回值说明

返回类型为ge::graphStatus：

- ge::GRAPH\_SUCCESS：InferShape成功。
- ge::GRAPH\_FAILED：InferShape失败。

## 约束说明

- 第0个输入为待归约张量，第1个输入为归约轴张量，轴数据类型为int32或int64。
- 第0个属性为bool类型的keep_dims。
- 仅适用于满足上述输入和属性布局的Reduce类算子；IsFinite等形状不变的逐元素算子应使用InferShape4Elewise。

## 调用示例

关键代码示例如下，仅供参考，不支持直接拷贝运行。

```cpp
// ReduceSum的输入为x、axes，第0个属性为keep_dims时，可复用Reduce形状推导。
ge::graphStatus InferShape4ReduceSum(gert::InferShapeContext* context)
{
    return Ops::Base::InferShape4Reduce(context);
}
// 将ReduceSum算子及其形状推导函数注册到GE。
IMPL_OP_INFERSHAPE(ReduceSum).InferShape(InferShape4ReduceSum);
```
