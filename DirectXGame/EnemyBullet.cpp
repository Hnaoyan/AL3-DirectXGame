#include "EnemyBullet.h"
#include"MathCalc.h"
#include<cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	// NULLCheck
	assert(model);

	model_ = model;

	this->textureHandle_ = TextureManager::Load("red.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;

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