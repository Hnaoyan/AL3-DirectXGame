#pragma once
#include<Vector3.h>
#include<Matrix4x4.h>

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
//Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// Rotate
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeInverse(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();

// Transpose
//Matrix4x4 Transpose(const Matrix4x4& m);
// Scale
Matrix4x4 MakeScaleMatrix(const Vector3& translate);
// Translate
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
