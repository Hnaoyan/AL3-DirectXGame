#include "Player.h"
#include<cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL Check
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Player::Update() {
	// Matrix to Transform
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3DModel
	model_->Draw(worldTransform_, viewProjection, this->textureHandle_);
}