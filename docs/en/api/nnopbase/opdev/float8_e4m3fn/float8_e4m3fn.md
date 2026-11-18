# float8_e4m3fn

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** API list

| API Definition| Description|
| --- | --- |
| FromBits() | Tag used to construct a Float8E4M3FN object from a bit pattern.|
| Float8E4M3FN() | Default constructor of struct Float8E4M3FN, initializing the object to zero.|
| Float8E4M3FN(uint8_t bits, FromBitsTag) | Constructor of struct Float8E4M3FN, constructing an object from a raw bit pattern.|
| Float8E4M3FN(float v) | Constructor of struct Float8E4M3FN, constructing an object from a float value.|
| operator float() | Converts Float8E4M3FN to float.|
| operator double() | Converts Float8E4M3FN to double.|
| Epsilon() | Returns the machine epsilon of the Float8E4M3FN data type.|
| Highest() | Returns the largest finite value of the Float8E4M3FN data type.|
| Lowest() | Returns the smallest finite value of the Float8E4M3FN data type.|
| MinPositiveNormal() | Returns the smallest positive normal value of the Float8E4M3FN data type.|
| IsZero() | Checks whether the current Float8E4M3FN value is zero.|
| IsNaN() | Checks whether the current Float8E4M3FN value is NaN.|
| IsInf() | Checks whether the current Float8E4M3FN value is infinity.|
| isinf(const Float8E4M3FN &a) | Checks whether the given Float8E4M3FN value a is infinity.|
| isnan(const Float8E4M3FN &a) | Checks whether the given Float8E4M3FN value a is NaN.|
| isfinite(const Float8E4M3FN &a) | Checks whether the given Float8E4M3FN value a is finite.|
| min() | Returns the smallest positive normal value of the Float8E4M3FN data type.|
| lowest() | Returns the smallest finite value of the Float8E4M3FN data type.|
| max() | Returns the largest finite value of the Float8E4M3FN data type.|
| epsilon() | Returns the machine epsilon of the Float8E4M3FN data type.|
| round_error() | Returns the maximum rounding error of the Float8E4M3FN data type.|
| infinity() | Returns the largest finite value of the Float8E4M3FN data type (this data type has no infinity).|
| quiet_NaN() | Returns a quiet NaN of the Float8E4M3FN data type.|
| signaling_NaN() | Returns a quiet NaN of the Float8E4M3FN data type (this data type has no signaling NaN).|
| denorm_min() | Returns the smallest positive denormal value of the Float8E4M3FN data type.|
