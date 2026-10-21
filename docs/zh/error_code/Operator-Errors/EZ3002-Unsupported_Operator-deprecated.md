# EZ3002 Unsupported\_Operator（已废弃）

## 错误信息

报错格式如下，占位符%s的含义依次为算子类型、算子信息库名称、报错原因：

```text
Optype [%s] of Ops kernel [%s] is unsupported. Reason: %s.
```

报错示例如下：

```text
Optype [Conv2D] of Ops kernel [AIcoreEngine] is unsupported. Reason: data type DT_FLOAT of input x is not supported.
```

## 解决方法

请在<https://gitee.com/ascend> 提交issue以请求支持，或从模型中移除该类型算子。
