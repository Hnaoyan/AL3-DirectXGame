#pragma once
#include <Vector3.h>
#include "Matrix4x4.h"

void MoveVector(Vector3& position, Vector3& vector);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
