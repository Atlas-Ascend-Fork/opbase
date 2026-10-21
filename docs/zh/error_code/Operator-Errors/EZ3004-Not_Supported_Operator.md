# EZ3004 Not\_Supported\_Operator

## 错误信息

报错格式如下，占位符%s的含义依次为算子信息库名称、算子名称、算子类型、报错原因：

```text
The operator information library %s does not support the operator %s (operator type: %s). Cause: %s.
```

报错示例如下：

```text
The operator information library AIcoreEngine does not support the operator Conv2D_1 (operator type: Conv2D). Cause: data type DT_FLOAT of input x is not supported.
```

## 解决方法

1. 如果使用的是内置算子，请升级到支持该规格的CANN算子包。
2. 如果使用的是自定义算子，请注册并使用满足所需规格的自定义算子。
