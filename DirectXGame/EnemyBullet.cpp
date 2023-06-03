#include "EnemyBullet.h"
#include"MathCalc.h"
#include<cassert>
#include<cmath>
#include "CollisionConfig.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	// NULLCheck
	assert(model);

	model_ = model;

	this->textureHandle_ = TextureManager::Load("red.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	velocity_ = velocity;
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float length = sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, length);
	//// 衝突属性を設定
	//SetCollisionAttribute(kCollisionAttributeEnemy);
	//// 衝突対象を自分の属性以外に設定
	//SetCollisionMask(kCollisionAttributePlayer);
}

void EnemyBullet::Update() {
	if (--deathTimer <= 0) {
		isDead_ = true;
	}

	// Move
	MoveVector(worldTransform_.translation_, velocity_);

	// Update
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { 
	this->model_->Draw(this->worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() { 
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
//// 衝突属性（自分）
//uint32_t EnemyBullet::GetCollisionAttribute() { return collisionAttribute_; }
//void EnemyBullet::SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
//
//// 衝突マスク（相手）
//uint32_t EnemyBullet::GetCollisionMask() { return collisionMask_; }
//void EnemyBullet::SetCollisionMask(uint32_t mask) { collisionMask_ = mask; }