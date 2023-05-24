#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include"Matrix.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"
#include "Collider.h"

class Player : public Collider
{
public:
	/// Initialize
	/// <param name="model">Model</param>
	/// <param name="textureHandle">Texture</param>
	void Initialize(Model* model,uint32_t textureHandle);

	/// Update
	void Update(ViewProjection& viewProjection);

	/// Draw
	/// <param name="viewProject">ViewProjection</param>
	void Draw(ViewProjection& viewProjection);

	void DrawUI();

	// Rotate
	void Rotate();

	// Attack
	void Attack();

	// Destract
	~Player();

	// Collision
	void OnCollision() override;

	// WorldPosition
	Vector3 GetWorldPosition() override;

	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	const float radius = 15.0f;

	// 親となるワールドトランスフォームをセット
	// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);

private:
	// WorldData
	WorldTransform worldTransform_;

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// Model
	Model* model_ = nullptr;

	Model* offsetModel_ = nullptr;

	// 2Dレティクル用
	Sprite* sprite2DReticle_ = nullptr;

	// Handle
	uint32_t textureHandle_ = 0u;

	uint32_t offsetTexture_ = 0u;

	// Keyboard
	Input* input_ = nullptr;

	// bullet
	std::list<PlayerBullet*> bullets_;
	//PlayerBullet* bullet_ = nullptr;

};
