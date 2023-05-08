#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"TextureManager.h"
#include <Input.h>
#include <list>
#include"EnemyBullet.h"
#include "TimedCall.h"

enum class Phase {
	Approach,	// Near
	Leave,		// Leave
};

class Enemy {
public:
	// Initialize
	void Initialize(Model* model);
	// Update
	void Update();
	// Draw
	void Draw(const ViewProjection& viewProjection);

	// Pattern
	void Approach();
	void Leave();

	// Bullet
	void Fire();

	void ApproachInitialize();

	~Enemy();

	static const int kFireInterval = 60;

	void FireTimeReset();

private:
	static void (Enemy::*spFuncTable[])();

	// world
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Texture
	uint32_t textureHandle_ = 0u;
	// Velocity
	Vector3 velocity_;
	// Phase	
	Phase phase_ = Phase::Approach;
	// Bullet
	std::list<EnemyBullet*> bullets_;
	// Keyboard
	Input* input_ = nullptr;

	int32_t shootTimer = 0;

	std::list<TimedCall*> timedCalls_;

};
