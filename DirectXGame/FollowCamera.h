#pragma once
#include "ViewProjection.h"
class FollowCamera {
public:

	void Initialize();

	void Update();

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;


};
