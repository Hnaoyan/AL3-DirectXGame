#pragma once
#include "Model.h"
#include "ViewProjection.h"

class Ground {
public:


	// Init
	void Initialize(Model* model);

	// Update
	void Update();

	// Draw
	void Draw(const ViewProjection& viewProjection);

private:

};
