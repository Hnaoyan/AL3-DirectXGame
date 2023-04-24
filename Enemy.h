#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"TextureManager.h"

enum class Phase {
	Normal,		// Normal
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

private:
	// world
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Texture
	uint32_t textureHandle_ = 0u;
	// Velocity
	Vector3 velocity_;
	// Phase
	Phase phase_ = Phase::Normal;
};
