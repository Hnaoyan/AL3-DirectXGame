#pragma once
#include "Vector3.h"
#include <Model.h>
#include <WorldTransform.h>

class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:

	WorldTransform worldTransform_;

	Model* model_;

	uint32_t textureHandle_ = 0u;
	// Velocity
	Vector3 velocity_;
	// Death
	static const int32_t kLifeTime = 60 * 5;
	// time
	int32_t deathTimer = kLifeTime;
	// flag
	bool isDead_ = false;
};
