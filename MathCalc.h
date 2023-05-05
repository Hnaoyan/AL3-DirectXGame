#pragma once
#include <Vector3.h>
#include "Matrix4x4.h"

void MoveVector(Vector3& position, Vector3& vector);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

class Calc 
{
public:
	static Vector3 Normalize(const Vector3& v);

};

const Vector3 operator+=(const Vector3& v1, const Vector3& v2);

const Vector3 operator+=(const Vector3& v1, const float speed);

const Vector3 operator-=(const Vector3& v1, const Vector3& v2);
