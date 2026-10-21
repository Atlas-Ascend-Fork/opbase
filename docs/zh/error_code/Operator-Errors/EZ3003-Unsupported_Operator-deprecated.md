# EZ3003 Unsupported\_Operator（已废弃）

## 错误信息

报错格式如下，占位符%s的含义依次为算子名称、算子类型：

```text
No supported Ops kernel and engine are found for [%s], optype [%s].
```

报错示例如下：

```text
No supported Ops kernel and engine are found for [Conv2D_1], optype [Conv2D].
```

## 解决方法

1. 检查OPP组件是否已正确安装。
2. 提交issue请求支持该算子类型。
