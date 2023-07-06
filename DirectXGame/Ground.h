#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Ground {
public:


	// Init
	void Initialize(Model* model);

	// Update
	void Update();

	// Draw
	void Draw(const ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;


};
