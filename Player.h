#pragma once
#include "Model.h"
#include "WorldTransform.h"

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

private:
	// WorldData
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Handle
	uint32_t textureHandle_ = 0u;
};
