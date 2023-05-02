#include "Enemy.h"
#include "MathCalc.h"
#include <assert.h>
#include "TextureManager.h"

void Enemy::Initialize(Model* model) { 
	assert(model);

	
	this->model_ = model;
	this->textureHandle_ = TextureManager::Load("white1x1.png"); 
	worldTransform_.Initialize();
	velocity_ = {0, 0, -0.5f};
	worldTransform_.translation_ = {0, 1.0f, 50.0f};
}

void (Enemy::*Enemy::spFuncTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::Approach() {
	// Move
	velocity_ = {0, 0, -0.5f};

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	// Move
	velocity_ = {-0.1f, 0.1f, -0.05f};

}

void Enemy::Update() {
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	//worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}