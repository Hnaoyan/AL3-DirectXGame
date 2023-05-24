#pragma once
#include "Vector3.h"

class Collider {
private:
	// 衝突半径
	float radius_ = 10.0f;

public:
	// 半径取得
	float GetterRad() { return radius_; }
	void SetterRad(float radius) { radius_ = radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision(){};
	virtual Vector3 GetWorldPosition() = 0; 

};
