#pragma once
#include <Vector3.h>
#include "Matrix4x4.h"

void MoveVector(Vector3& position, Vector3& vector);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

//class Calc 
//{
//public:
//	inline Vector3& operator+=(const Vector3& v1) 
//	{
//		vector += v1;
//		return *this;
//	}
//
//private:
//	Vector3 vector;
//};