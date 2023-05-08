#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"BaseEnemyState.h"
#include"TextureManager.h"

enum class Phase {
	Approach,	// Near
	Leave,		// Leave
};
class Enemy;

class BaseEnemyState {
protected:
	Enemy* enemy_ = nullptr;

public:
	virtual void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	virtual void Update(){};

};

class EnemyApproach : public BaseEnemyState
{
public:
	void Update();
};

class EnemyLeave : public BaseEnemyState
{
public:
	void Update();
};

class Enemy {
public:
	// Initialize
	void Initialize(Model* model);
	// Update
	void Update();
	// Draw
	void Draw(const ViewProjection& viewProjection);

	// State
	void ChangeState(BaseEnemyState* state);

	void MoveUpdate(Vector3 velocity);

	Vector3 GetWorldPos() { return this->worldTransform_.translation_; }
	~Enemy();

private:
	BaseEnemyState* state_;

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
