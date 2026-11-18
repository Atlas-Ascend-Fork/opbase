# hifloat8

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** API list

| API Definition| Description|
| --- | --- |
| FromBits() | Tag used to construct a HiFloat8 object from a bit pattern.|
| HiFloat8() | Default constructor of struct HiFloat8, initializing the object to zero.|
| HiFloat8(uint8_t bits, FromBitsTag) | Constructor of struct HiFloat8, constructing an object from a raw bit pattern.|
| HiFloat8(float f32) | Constructor of struct HiFloat8, constructing an object from a float value.|
| operator float() | Converts HiFloat8 to float.|
| IsNaN() | Checks whether the current HiFloat8 value is NaN.|
| IsInf() | Checks whether the current HiFloat8 value is infinity.|
| IsZero() | Checks whether the current HiFloat8 value is zero.|
| BitsFromFp32(uint32_t f32) | Converts the FP32 bit pattern to the HiFloat8 bit pattern.|
| isinf(const HiFloat8 &a) | Checks whether the given HiFloat8 value a is infinity.|
| isnan(const HiFloat8 &a) | Checks whether the given HiFloat8 value a is NaN.|
| isfinite(const HiFloat8 &a) | Checks whether the given HiFloat8 value a is finite.|
| min() | Returns the smallest positive normal value of the HiFloat8 data type.|
| lowest() | Returns the smallest finite value of the HiFloat8 data type.|
| max() | Returns the largest value of the HiFloat8 data type.|
| epsilon() | Returns the machine epsilon of the HiFloat8 data type.|
| round_error() | Returns the maximum rounding error of the HiFloat8 data type.|
| infinity() | Returns the infinity of the HiFloat8 data type.|
| quiet_NaN() | Returns a quiet NaN of the HiFloat8 data type.|
| signaling_NaN() | Returns a signaling NaN of the HiFloat8 data type.|
| denorm_min() | Returns the smallest positive denormal value of the HiFloat8 data type.|
