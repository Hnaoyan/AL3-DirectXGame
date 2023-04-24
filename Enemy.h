#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"TextureManager.h"

class Enemy {
public:
	// Initialize
	void Initialize(Model* model);
	// Update
	void Update();
	// Draw
	void Draw(const ViewProjection& viewProjection);

private:
	// world
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Texture
	uint32_t textureHandle_ = 0u;
	// velocity
	Vector3 velocity_;
};
