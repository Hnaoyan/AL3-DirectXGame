#include "MathCalc.h"

void MoveVector(Vector3& position, Vector3& vector) {
	position.x += vector.x;
	position.y += vector.y;
	position.z += vector.z;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) 
{
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;

}

const Vector3 operator+=(const Vector3& v1, const Vector3& v2) { 
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

const Vector3 operator+=(const Vector3& v1, const float speed) {
	Vector3 result;
	result.x = v1.x + speed;
	result.y = v1.y + speed;
	result.z = v1.z + speed;
	return result;
}

const Vector3 operator-=(const Vector3& v1, const Vector3& v2) { 
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}