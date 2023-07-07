#include "Vector3Utils.h"

Vector3 Scaler(const Vector3& v1, const float scaler) { 
	Vector3 result;
	result.x = v1.x * scaler;
	result.y = v1.y * scaler;
	result.z = v1.z * scaler;
	return result; 
}
