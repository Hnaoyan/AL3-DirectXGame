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
	state_ = new EnemyApproach();
	state_->SetEnemy(this);
}

Enemy::~Enemy() { delete state_; }

void EnemyApproach::Update() {
	// Move
	Vector3 velocity = {0, 0, -0.5f};
	enemy_->MoveUpdate(velocity);
	if (enemy_->GetWorldPos().z < 0.0f) {
		enemy_->ChangeState(new EnemyLeave());
	}
}

void EnemyLeave::Update() {
	// Move
	Vector3 velocity = {-0.1f, 0.1f, -0.05f};
	enemy_->MoveUpdate(velocity);
}

void Enemy::MoveUpdate(Vector3 velocity) {
	worldTransform_.translation_ = worldTransform_.translation_ + velocity;
}

void Enemy::ChangeState(BaseEnemyState* newState) { 
	//delete state_;
	state_ = newState;
	state_->SetEnemy(this);
}

void Enemy::Update() {

	state_->Update();
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
