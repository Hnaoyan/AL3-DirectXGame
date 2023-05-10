#include "Enemy.h"
#include "MathCalc.h"
#include <assert.h>
#include "TextureManager.h"

Enemy::~Enemy() { 
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	for (TimedCall* timecalls : timedCalls_) {
		delete timecalls;
	}
}

void Enemy::Initialize(Model* model) { 
	assert(model);

	input_ = Input::GetInstance();
	this->model_ = model;
	this->textureHandle_ = TextureManager::Load("white1x1.png"); 
	worldTransform_.Initialize();
	velocity_ = {0, 0, -0.5f};
	worldTransform_.translation_ = {10.0f, 1.0f, 100.0f};
	ApproachInitialize();
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

	timedCalls_.remove_if([](TimedCall* timecalls) {
		if (timecalls->IsFinished()) {
			delete timecalls;
			return true;
		}
		return false;
	});

	for (TimedCall* timedCalls : timedCalls_) {
		timedCalls->Update();
	}

	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.UpdateMatrix();

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
	// Velocity
	const float kBulletSpeed = -0.5f;
	Vector3 velocity = {0, 0, kBulletSpeed};
	// Initialize
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, this->worldTransform_.translation_,velocity);

	bullets_.push_back(newBullet);
}

void (Enemy::*Enemy::spFuncTable[])() = 
{
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::Approach() {
	//shootTimer--;
	//if (shootTimer == 0) {
	//	// Fire
	//	Fire();
	//	// TimerSet
	//	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireTimeReset, this), this->kFireInterval));

	//	// Init
	//	shootTimer = kFireInterval;
	//}

	// Move
	velocity_ = {0, 0, -0.1f};

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	// Move
	velocity_ = {-0.1f, 0.1f, -0.05f};

}

void Enemy::ApproachInitialize() {

	this->shootTimer = this->kFireInterval;
	
}

void Enemy::FireTimeReset() {
	// Fire
	Fire();

	// reset
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireTimeReset, this), shootTimer));
}