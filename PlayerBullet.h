#pragma once
#include "Model.h"
#include "WorldTransform.h"

// PlayerCharacterBullet
class PlayerBullet {
public:
	/// Initialize
	/// <param name="model">model</param>
	/// <param name="position">initialPosition</param>
	/// <param name="velocity">Velocity</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// Update
	void Update();

	/// Draw
	/// <param name="ViewProjection">ViewProjection</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:
	// WorldData
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Handle
	uint32_t textureHandle_ = 0u;
	// Velocity
	Vector3 velocity_;
	// LifeTime
	static const int32_t kLifeTime = 60 * 5;
	// DeathTimer
	int32_t deathTimer_ = kLifeTime;
	// DeathFlag
	bool isDead_ = false;
};
