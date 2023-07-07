#include "Vector3Utils.h"

Vector3 Scaler(const Vector3& v1, const float scaler) { 
	Vector3 result;
	result.x = v1.x * scaler;
	result.y = v1.y * scaler;
	result.z = v1.z * scaler;
	return result; 
}

Vector3 Add(const Vector3 v1, const Vector3 v2) { 
	Vector3 v{};

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;

	return v; 
}
