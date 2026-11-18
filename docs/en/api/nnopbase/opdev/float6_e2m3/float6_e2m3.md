# float6_e2m3

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** API list

| API Definition| Description|
| --- | --- |
| FromBits() | Tag used to construct a Float6E2M3 object from a bit pattern.|
| Float6E2M3() | Default constructor of struct Float6E2M3, initializing the object to zero.|
| Float6E2M3(uint8_t bits, FromBitsTag) | Constructor of struct Float6E2M3, constructing an object from a raw bit pattern.|
| Float6E2M3(float v) | Constructor of struct Float6E2M3, constructing an object from a float value.|
| operator float() | Converts Float6E2M3 to float.|
| operator double() | Converts Float6E2M3 to double.|
| Epsilon() | Returns the machine epsilon of the Float6E2M3 data type.|
| Highest() | Returns the largest finite value of the Float6E2M3 data type.|
| Lowest() | Returns the smallest finite value of the Float6E2M3 data type.|
| MinPositiveNormal() | Returns the smallest positive normal value of the Float6E2M3 data type.|
| IsZero() | Checks whether the current Float6E2M3 value is zero.|
| IsNaN() | Checks whether the current Float6E2M3 value is NaN.|
| IsInf() | Checks whether the current Float6E2M3 value is infinity.|
| isinf(const Float6E2M3 &a) | Checks whether the given Float6E2M3 value a is infinity.|
| isnan(const Float6E2M3 &a) | Checks whether the given Float6E2M3 value a is NaN.|
| isfinite(const Float6E2M3 &a) | Checks whether the given Float6E2M3 value a is finite.|
| min() | Returns the smallest positive normal value of the Float6E2M3 data type.|
| lowest() | Returns the smallest finite value of the Float6E2M3 data type.|
| max() | Returns the largest finite value of the Float6E2M3 data type.|
| epsilon() | Returns the machine epsilon of the Float6E2M3 data type.|
| round_error() | Returns the maximum rounding error of the Float6E2M3 data type.|
| infinity() | Returns the largest finite value of the Float6E2M3 data type (this data type has no infinity).|
| quiet_NaN() | Returns zero of the Float6E2M3 data type (this data type has no quiet NaN).|
| signaling_NaN() | Returns zero of the Float6E2M3 data type (this data type has no signaling NaN).|
| denorm_min() | Returns the smallest positive denormal value of the Float6E2M3 data type.|
