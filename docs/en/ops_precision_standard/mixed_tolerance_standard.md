# Open-Source Precision Standard for Ecosystem Operators

## 0. Scope Description

This standard applies to **floating-point computation operators**. For integer computation operators and data movement operators, separate standards must be formulated based on the actual business scenarios of each operator, which are outside the scope of this standard.

---

## 1. Test Case Generation Rules

The inputs of an ecosystem operator mainly consist of two parts: tensors and attributes (Attr). Test cases must cover the typical scenarios and boundary scenarios of the operator in all scenarios to ensure the completeness of precision evaluation.

### 1.1 Test Case Scale Requirements

| Requirement | Description |
|--------|------|
| **Coverage Objective** | Implement **full-combination coverage** (that is, 100% coverage of all valid combinations) of the **data types**, **data formats**, **data dimensions**, and **attribute value ranges** supported by the operator. |
| **Data Type Coverage** | Cover all data types supported by the operator. |

> Note: No fixed lower limit is set for the number of test cases (for example, ≥1,000). The emphasis is on the traversal coverage of input combinations rather than a mechanical requirement for the number of test cases.

### 1.2 Tensor Generation Rules

| Dimension | Generation Rule |
|------|----------|
| **Data Dimension** | Cover the dimension range supported by the operator (1 to 8 dimensions). Dimension values take two kinds of values within $[1, 2^{20}]$: powers of 2 and powers of 2 minus 1. The total number of elements does not exceed 2 to the 34th power. |
| **Data Format** | Cover all data formats supported by the operator (ND, NCHW, etc.). |

**Stride and combination generation**: Orthogonal combination traversal should be performed among the parameter types, and the value range coverage strategy is as follows:

| Distribution Type | Proportion | Value Range | Generation Logic |
|---------|------|----------|----------|
| Uniform distribution | 50% | [-5, 5] | Random uniform sampling |
| Normal distribution | 50% | μ ∈ [-5, 5], σ ∈ [0.1, 2] | Generated after randomly sampling μ and σ |

### 1.3 Attr Generation Rules

| Parameter Type | Generation Rule |
|----------|----------|
| Scalar parameter | Cover all equivalence class scenarios. |
| Boolean parameter | Cover True and False. |
| Enumeration parameter | Cover all supported enumeration values. |

The various parameter types should be traversed in combination.

### 1.4 Special Scenario Tests

| Scenario | Description | Coverage Requirement |
|------|------|----------|
| Empty Tensor | Tensor with a dimension of 0 | Each tensor of each dtype is covered at least once. |
| Scalar Tensor | Tensor whose shape is [1] | Covered for each dtype. |
| Boundary Test | Lower boundary: all dimensions of the shape are 1; upper boundary: a dimension of the shape takes the maximum value | Fully covered. |
| INF/-INF/NAN | Input element values traverse nan, inf, -inf, and [-inf, inf] | Different shape cases are generated for each value of each dtype. |

> Note: Special scenario test cases are not orthogonally combined with regular test cases.

---

## 2. Error Metrics and Passing Criteria

### 2.1 Error Metrics

This standard uses the **Mixed Tolerance** metric for judgment, which combines absolute tolerance (atol) and relative tolerance (rtol) in an element-wise comparison manner.

#### 2.1.1 Element-wise Passing Conditions

For each element in the output tensor, the element is determined to pass when the following condition is met:

$$
|actual - golden| \leq atol + rtol \times |golden|
$$

Where:

- **atol** (Absolute Tolerance): ensures a reasonable error range for small values (golden close to 0), naturally avoiding the division-by-zero problem.
- **rtol** (Relative Tolerance): ensures relative precision for large-value scenarios.

#### 2.1.2 Overall Passing Conditions

Define the **matched ratio** as the ratio of the number of passed elements to the total number of elements:

$$
\text{matched\_ratio} = \frac{\text{passed elements}}{\text{total elements}}
$$

When both of the following conditions are met, the test case is determined to pass:

(1) `matched_ratio ≥ required_matched_ratio`  
(2) `max_abs_error ≤ max_abs_error_limit`

Where:

- `max_abs_error` is the maximum absolute error of all **finite value comparison points** (both actual and golden are finite values) in the test case. ±inf/NaN elements do not participate in the selection of this value and are compared according to the narrowing rules in section 2.3.
- `max_abs_error_limit` is the hard upper limit of the absolute error, dynamically calculated based on the value range where the maximum absolute error point is located: `max_abs_error_limit = max(fallback value, 32 * ULP(g_low))`, where `g_low` is the value obtained by narrowing the golden value at the maximum absolute error point according to the actual output dtype of the operator (see the threshold table in section 2.2 for the fallback value).

### 2.2 Mixed Tolerance Threshold Table

<table style="width: 100%; border-collapse: collapse;">
    <thead>
      <tr>
        <th style="text-align: center; border: 1px solid #ddd; padding: 8px;">Data Type</th>
        <th style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>FLOAT16</strong></th>
        <th style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>BFLOAT16</strong></th>
        <th style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>FLOAT32</strong></th>
        <th style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>HiFLOAT32</strong></th>
        <th style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>FLOAT8 E4M3</strong></th>
        <th style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>FLOAT8 E5M2</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>rtol</strong></td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-10</sup> (1e-3)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-7</sup> (7.8e-3)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-13</sup> (1.2e-4)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-10</sup> (1e-3)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-3</sup> (0.125)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-2</sup> (0.25)</td>
      </tr>
      <tr>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>atol</strong></td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-10</sup> (1e-3)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-7</sup> (7.8e-3)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-13</sup> (1.2e-4)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-10</sup> (1e-3)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-3</sup> (0.125)</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">2<sup>-2</sup> (0.25)</td>
      </tr>
      <tr>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>required_matched_ratio</strong></td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;" colspan="6">0.99</td>
      </tr>
      <tr>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;"><strong>max_abs_error_limit</strong></td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">1e-1 or 32 * ULP</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">1e-1 or 32 * ULP</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">1e-2 or 32 * ULP</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">1e-1 or 32 * ULP</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">1e-0 or 32 * ULP</td>
        <td style="text-align: center; border: 1px solid #ddd; padding: 8px;">1e-0 or 32 * ULP</td>
      </tr>
    </tbody>
</table>

**Note**:

- At present, the threshold interception is about 1 ULP for each numerical precision. For scenarios involving large-number reduction, a larger precision error may be introduced and cause false positives, so it can be lowered to 2 ULP as appropriate.
- In the `max_abs_error_limit` row of the table, `or` means taking the larger of the two terms. The anchor and calculation basis of `ULP` (adaptively scaled with the tested dtype and the value range of the maximum error point) are defined in section 2.1.2.

### 2.3 Passing Determination

**Single benchmark comparison**: Directly compare with a single precision benchmark implemented with higher precision (CPU or Ascend small-operator concatenation).

When a test case satisfies both `matched_ratio ≥ required_matched_ratio` and `max_abs_error ≤ max_abs_error_limit`, the precision of the test case is determined to pass.

During the comparison, if the actual operator output is `±inf` while golden is a finite value, golden should first be reduced in precision (narrowed) to the actual output dtype of the operator before comparison: narrowing follows round-to-nearest-even rounding, and values beyond the representation range of that dtype are converted to `±inf` (IEEE 754 semantics). After narrowing, if both sides are `±inf` with the same sign, the point is regarded as passed.
