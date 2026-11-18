# float8_e8m0

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** API list

| API Definition| Description|
| --- | --- |
| FromBits() | Tag used to construct a Float8E8M0 object from a bit pattern.|
| Float8E8M0() | Default constructor of struct Float8E8M0, initializing the object to zero.|
| Float8E8M0(uint8_t bits, FromBitsTag) | Constructor of struct Float8E8M0, constructing an object from a raw bit pattern.|
| Float8E8M0(float v) | Constructor of struct Float8E8M0, constructing an object from a float value.|
| operator float() | Converts Float8E8M0 to float.|
| operator double() | Converts Float8E8M0 to double.|
| Epsilon() | Returns the machine epsilon of the Float8E8M0 data type.|
| Highest() | Returns the largest value of the Float8E8M0 data type.|
| Lowest() | Returns the smallest positive value of the Float8E8M0 data type (unsigned format).|
| MinPositive() | Returns the smallest positive value of the Float8E8M0 data type.|
| IsZero() | Checks whether the current Float8E8M0 value is zero.|
| IsNaN() | Checks whether the current Float8E8M0 value is NaN.|
| IsInf() | Checks whether the current Float8E8M0 value is infinity.|
| GetExponent() | Obtains the unbiased exponent of the current Float8E8M0 value.|
| isinf(const Float8E8M0 &a) | Checks whether the given Float8E8M0 value a is infinity.|
| isnan(const Float8E8M0 &a) | Checks whether the given Float8E8M0 value a is NaN.|
| isfinite(const Float8E8M0 &a) | Checks whether the given Float8E8M0 value a is finite.|
| min() | Returns the smallest positive value of the Float8E8M0 data type.|
| lowest() | Returns the smallest positive value of the Float8E8M0 data type (unsigned format).|
| max() | Returns the largest value of the Float8E8M0 data type.|
| epsilon() | Returns the machine epsilon of the Float8E8M0 data type.|
| round_error() | Returns the maximum rounding error of the Float8E8M0 data type.|
| infinity() | Returns the largest finite value of the Float8E8M0 data type (this data type has no infinity).|
| quiet_NaN() | Returns a quiet NaN of the Float8E8M0 data type.|
| signaling_NaN() | Returns a quiet NaN of the Float8E8M0 data type (this data type has no signaling NaN).|
| denorm_min() | Returns the smallest positive denormal value of the Float8E8M0 data type.|
