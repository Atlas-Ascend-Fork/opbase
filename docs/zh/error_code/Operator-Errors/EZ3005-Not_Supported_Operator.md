# EZ3005 Not\_Supported\_Operator

## 错误信息

报错格式如下，占位符%s的含义依次为算子名称、算子类型：

```text
No operator information library that supports operator %s (operator type: %s) can be found.
```

报错示例如下：

```text
No operator information library that supports operator Conv2D_1 (operator type: Conv2D) can be found.
```

## 可能原因

系统不存在支持该算子的算子信息库。

## 解决方法

1. 如果使用的是内置算子，请升级到支持该算子的CANN算子包。
2. 如果使用的是自定义算子，请开发并注册该算子。
