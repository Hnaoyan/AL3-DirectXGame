#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"BaseEnemyState.h"
#include"TextureManager.h"

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

	// State
	//void ChangeState(BaseEnemyState* state);

	//void MoveUpdate(Vector3& velocity);

private:
	static void (Enemy::*spFuncTable[])();
	//BaseEnemyState* state_;


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
};
