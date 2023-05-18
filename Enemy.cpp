#include "Enemy.h"
#include "MathCalc.h"
#include <assert.h>
#include "TextureManager.h"
#include "Player.h"
#include "GameScene.h"

Enemy::~Enemy() { 
	//for (EnemyBullet* bullet : enemyBullets_) {
	//	delete bullet;
	//}
}

void Enemy::Initialize(Model* model,GameScene* gameScene, Vector3 position) { 
	assert(model);

	input_ = Input::GetInstance();
	this->model_ = model;
	this->textureHandle_ = TextureManager::Load("white1x1.png"); 

	worldTransform_.Initialize();
	velocity_ = {0, 0, -0.5f};
	//worldTransform_.translation_ = {10.0f, 1.0f, 100.0f};
	worldTransform_.translation_ = position;
	shootTimer = kFireInterval;

	SetGameScene(gameScene);

	ApproachInitialize();

}

void Enemy::Update() {

	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

Vector3 Enemy::GetWolrdPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Fire() { 
	assert(player_);

	// Velocity
	const float kBulletSpeed = 0.5f;
	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = this->GetWolrdPosition();

	Vector3 vectorDiff = playerPosition - enemyPosition;
	vectorDiff = Calc::Normalize(vectorDiff);

	Vector3 velocity = {
	    vectorDiff.x * kBulletSpeed, vectorDiff.y * kBulletSpeed, vectorDiff.z * kBulletSpeed};
	// Initialize
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, this->worldTransform_.translation_,velocity);
	
	gameScene_->AddEnemyBullet(newBullet);
}

void (Enemy::*Enemy::spFuncTable[])() = 
{
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::Approach() {
	shootTimer--;
	if (shootTimer == 0) {
		// Fire
		Fire();
		// Init
		shootTimer = kFireInterval;
	}

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

void Enemy::OnCollision() { 
	isDead_ = true;
}