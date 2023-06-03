#pragma once
#include "Vector3.h"

class Collider {
private:
	// 衝突半径
	float radius_ = 10.0f;

	//// 衝突属性（自分）
	//uint32_t collisionAttribute_;
	//// 衝突マスク（相手）
	//uint32_t collisionMask_;

public:
	// 半径取得
	float GetterRad() { return radius_; }
	void SetterRad(float radius) { radius_ = radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision(){};
	virtual Vector3 GetWorldPosition() = 0; 

	//// 衝突属性（自分）を取得
	//virtual uint32_t GetCollisionAttribute() {}
	//// 衝突属性（自分）を設定
	//virtual void SetCollisionAttribute(uint32_t attribute) {}
	//// 衝突マスク（相手）を取得
	//virtual uint32_t GetCollisionMask() {}
	//// 衝突マスク（相手）を設定
	//virtual void SetCollisionMask(uint32_t mask) {}
};
