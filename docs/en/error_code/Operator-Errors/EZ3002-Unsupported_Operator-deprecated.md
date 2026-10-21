# EZ3002 Unsupported\_Operator (deprecated)

## Symptom

The following is error format. The meanings of the placeholders %s in sequence are: operator type, operator information library, error cause.

```text
Optype [%s] of Ops kernel [%s] is unsupported. Reason: %s.
```

Error example:

```text
Optype [Conv2D] of Ops kernel [AIcoreEngine] is unsupported. Reason: data type DT_FLOAT of input x is not supported.
```

## Solution

Submit an issue to request for support at <https://gitee.com/ascend>, or remove this type of operators from your model.
