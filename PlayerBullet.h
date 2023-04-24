#pragma once
#include "Model.h"
#include "WorldTransform.h"

// PlayerCharacterBullet
class PlayerBullet {
public:
	/// Initialize
	/// <param name="model">model</param>
	/// <param name="position">initialPosition</param>
	void Initialize(Model* model, const Vector3& position);

	/// Update
	void Update();

	/// Draw
	/// <param name="ViewProjection">ViewProjection</param>
	void Draw(const ViewProjection& viewProjection);

private:
	// WorldData
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Handle
	uint32_t textureHandle_ = 0u;

};
