#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	// NULLPointerCheck
	assert(model);

	this->model_ = model;
	// TextureLoad
	this->textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	// InitialPositionReset
	this->worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	this->worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// ModelDraw
	this->model_->Draw(worldTransform_, viewProjection, textureHandle_);
}