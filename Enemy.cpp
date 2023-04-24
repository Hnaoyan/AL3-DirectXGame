#include "Enemy.h"
#include "Vector.h"
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

void Enemy::Update() {
	
	MoveVector(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}