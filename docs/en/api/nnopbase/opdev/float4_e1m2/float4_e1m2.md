# float4_e1m2

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** API list

| API Definition| Description|
| --- | --- |
| FromBits() | Tag used to construct a Float4E1M2 object from a bit pattern.|
| Float4E1M2() | Default constructor of struct Float4E1M2, initializing the object to zero.|
| Float4E1M2(uint8_t bits, FromBitsTag) | Constructor of struct Float4E1M2, constructing an object from a raw bit pattern.|
| Float4E1M2(float v) | Constructor of struct Float4E1M2, constructing an object from a float value.|
| operator float() | Converts Float4E1M2 to float.|
| operator double() | Converts Float4E1M2 to double.|
| Epsilon() | Returns the machine epsilon of the Float4E1M2 data type.|
| Highest() | Returns the largest finite value of the Float4E1M2 data type.|
| Lowest() | Returns the smallest finite value of the Float4E1M2 data type.|
| MinPositiveNormal() | Returns the smallest positive normal value of the Float4E1M2 data type.|
| IsZero() | Checks whether the current Float4E1M2 value is zero.|
| IsNaN() | Checks whether the current Float4E1M2 value is NaN.|
| IsInf() | Checks whether the current Float4E1M2 value is infinity.|
| isinf(const Float4E1M2 &a) | Checks whether the given Float4E1M2 value a is infinity.|
| isnan(const Float4E1M2 &a) | Checks whether the given Float4E1M2 value a is NaN.|
| isfinite(const Float4E1M2 &a) | Checks whether the given Float4E1M2 value a is finite.|
| min() | Returns the smallest positive normal value of the Float4E1M2 data type.|
| lowest() | Returns the smallest finite value of the Float4E1M2 data type.|
| max() | Returns the largest finite value of the Float4E1M2 data type.|
| epsilon() | Returns the machine epsilon of the Float4E1M2 data type.|
| round_error() | Returns the maximum rounding error of the Float4E1M2 data type.|
| infinity() | Returns the largest finite value of the Float4E1M2 data type (this data type has no infinity).|
| quiet_NaN() | Returns zero of the Float4E1M2 data type (this data type has no quiet NaN).|
| signaling_NaN() | Returns zero of the Float4E1M2 data type (this data type has no signaling NaN).|
| denorm_min() | Returns the smallest positive denormal value of the Float4E1M2 data type.|
