# EZ3003 Unsupported\_Operator (deprecated)

## Symptom

The following is error format. The meanings of the placeholders %s in sequence are: operator name, operator type.

```text
No supported Ops kernel and engine are found for [%s], optype [%s].
```

Error example:

```text
No supported Ops kernel and engine are found for [Conv2D_1], optype [Conv2D].
```

## Solution

1. Check that the OPP component is installed properly.
2. Submit an issue to request for the support of this operator type.
