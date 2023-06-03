#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"

// PlayerCharacterBullet
class PlayerBullet : public Collider
{
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
	// Collision
	void OnCollision() override;

	Vector3 GetWorldPosition() override;

	const float radius = 10.0f;

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

//private:
//	// 衝突属性（自分）
//	uint32_t collisionAttribute_ = 0xffffffff;
//	// 衝突マスク（相手）
//	uint32_t collisionMask_ = 0xffffffff;
//
//public:
//	// 衝突属性（自分）を取得
//	uint32_t GetCollisionAttribute() override;
//	// 衝突属性（自分）を設定
//	void SetCollisionAttribute(uint32_t attribute) override;
//	// 衝突マスク（相手）を取得
//	uint32_t GetCollisionMask() override;
//	// 衝突マスク（相手）を設定
//	void SetCollisionMask(uint32_t mask) override;
};
