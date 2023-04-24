#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include"Matrix.h"
#include "PlayerBullet.h"

class Player
{
public:
	/// Initialize
	/// <param name="model">Model</param>
	/// <param name="textureHandle">Texture</param>
	void Initialize(Model* model,uint32_t textureHandle);

	/// Update
	void Update();

	/// Draw
	/// <param name="viewProject">ViewProjection</param>
	void Draw(ViewProjection& viewProjection);

	void MoveVector(Vector3& position, Vector3& vector);

	// Rotate
	void Rotate();

	// Attack
	void Attack();

	private:
	// WorldData
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Handle
	uint32_t textureHandle_ = 0u;

	// Keyboard
	Input* input_ = nullptr;

	// bullet
	PlayerBullet* bullet_ = nullptr;

};
