# hifloat4

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** API list

| API Definition| Description|
| --- | --- |
| FromBits() | Tag used to construct a HiFloat4 object from a bit pattern.|
| HiFloat4() | Default constructor of struct HiFloat4, initializing the object to zero.|
| HiFloat4(uint8_t bits, FromBitsTag) | Constructor of struct HiFloat4, constructing an object from a raw bit pattern.|
| HiFloat4(float v) | Constructor of struct HiFloat4, constructing an object from a float value.|
| operator float() | Converts HiFloat4 to float.|
| operator double() | Converts HiFloat4 to double.|
| Epsilon() | Returns the machine epsilon of the HiFloat4 data type.|
| Highest() | Returns the largest finite value of the HiFloat4 data type.|
| Lowest() | Returns the smallest finite value of the HiFloat4 data type.|
| MinPositiveNormal() | Returns the smallest positive normal value of the HiFloat4 data type.|
| IsZero() | Checks whether the current HiFloat4 value is zero.|
| IsNaN() | Checks whether the current HiFloat4 value is NaN.|
| IsInf() | Checks whether the current HiFloat4 value is infinity.|
| isinf(const HiFloat4 &a) | Checks whether the given HiFloat4 value a is infinity.|
| isnan(const HiFloat4 &a) | Checks whether the given HiFloat4 value a is NaN.|
| isfinite(const HiFloat4 &a) | Checks whether the given HiFloat4 value a is finite.|
| min() | Returns the smallest positive normal value of the HiFloat4 data type.|
| lowest() | Returns the smallest finite value of the HiFloat4 data type.|
| max() | Returns the largest finite value of the HiFloat4 data type.|
| epsilon() | Returns the machine epsilon of the HiFloat4 data type.|
| round_error() | Returns the maximum rounding error of the HiFloat4 data type.|
| infinity() | Returns the largest finite value of the HiFloat4 data type (this data type has no infinity).|
| quiet_NaN() | Returns zero of the HiFloat4 data type (this data type has no quiet NaN).|
| signaling_NaN() | Returns zero of the HiFloat4 data type (this data type has no signaling NaN).|
| denorm_min() | Returns the smallest positive denormal value of the HiFloat4 data type.|
