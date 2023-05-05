#include "EnemyBullet.h"
#include"MathCalc.h"
#include<cassert>
#include<cmath>

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