#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// カメラのリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 追従ターゲット設定
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target) {
		target_ = target;
		Reset();
	}

	ViewProjection GetView() { return viewProjection_; }

	ViewProjection* GetViewPlayer() { return &viewProjection_; }

private:
	// 追従対象
	const WorldTransform* target_ = nullptr;
	
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// オフセットを設定
	Vector3 SetOffset() const;

	Vector3 defaultOffset = {0.0f, 2.0f, -10.0f};

	Vector3 interTarget_ = {};

	// 目標角度
	float destinationAngleY_ = 0.0f;

	float t = 0.01f;

};
