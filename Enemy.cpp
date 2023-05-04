#include "Enemy.h"
#include "MathCalc.h"
#include <assert.h>
#include "TextureManager.h"

Enemy::~Enemy() { 
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model) { 
	assert(model);

	input_ = Input::GetInstance();
	this->model_ = model;
	this->textureHandle_ = TextureManager::Load("white1x1.png"); 
	worldTransform_.Initialize();
	velocity_ = {0, 0, -0.5f};
	worldTransform_.translation_ = {0, 1.0f, 50.0f};
}

void Enemy::Update() {
	// BulletDeathFlag
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//MoveVector(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();

	ApproachInitialize();

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		// Velocity
		const float kBulletSpeed = -0.5f;
		Vector3 velocity = {0, 0, kBulletSpeed};
		// Initialize
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, this->worldTransform_.translation_,velocity);

		bullets_.push_back(newBullet);
	}
}

void (Enemy::*Enemy::spFuncTable[])() = 
{
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::Approach() {
	// Move
	velocity_ = {0, 0, -0.1f};

	worldTransform_.translation_ += velocity_;

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	// Move
	velocity_ = {-0.1f, 0.1f, -0.05f};

	worldTransform_.translation_ += velocity_;
}

void Enemy::ApproachInitialize() {

	this->shootTimer = this->kFireInterval;

}