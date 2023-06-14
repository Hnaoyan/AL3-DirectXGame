#pragma once
#include "Vector3.h"

class Collider {
private:
	// 衝突半径
	float radius_ = 10.0f;

	// 衝突属性（自分）
	int collisionAttribute_ = 0xffffffff;
	// 衝突マスク（相手）
	int collisionMask_ = 0xffffffff;

public:
	// 半径取得
	float GetterRad() { return radius_; }
	void SetterRad(float radius) { radius_ = radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision(){};
	virtual Vector3 GetWorldPosition() = 0; 

	// 衝突属性（自分）を取得
	virtual int GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突属性（自分）を設定
	virtual void SetCollisionAttribute(int attribute) { collisionAttribute_ = attribute; }
	// 衝突マスク（相手）を取得
	virtual int GetCollisionMask() { return collisionMask_; }
	// 衝突マスク（相手）を設定
	virtual void SetCollisionMask(int mask) { collisionMask_ = mask; }
};
