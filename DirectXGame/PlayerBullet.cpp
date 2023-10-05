#include "PlayerBullet.h"
#include <assert.h>
#include"MathCalc.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLPointerCheck
	assert(model);

	this->model_ = model;
	// TextureLoad
	this->textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	// InitialPositionReset
	this->worldTransform_.translation_ = position;

	// 
	this->velocity_ = velocity;
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionAttributeEnemy);
}

void PlayerBullet::Update() { 
	// Timer
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// positionMove
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	this->worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// ModelDraw
	this->model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() {
	isDead_ = true;

}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
