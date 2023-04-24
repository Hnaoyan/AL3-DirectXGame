#include "PlayerBullet.h"
#include <assert.h>
#include"MathCalc.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLPointerCheck
	assert(model);

	this->model_ = model;
	// TextureLoad
	this->textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	// InitialPositionReset
	this->worldTransform_.translation_ = position;
	// 
	this->velocity_ = velocity;
}

void PlayerBullet::Update() { 
	// Timer
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// positionMove
	MoveVector(worldTransform_.translation_, velocity_);
	this->worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// ModelDraw
	this->model_->Draw(worldTransform_, viewProjection, textureHandle_);
}