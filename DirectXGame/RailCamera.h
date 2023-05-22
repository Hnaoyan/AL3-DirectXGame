#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
///  レールカメラ
/// </summary>
class RailCamera 
{
public:
	// 初期化
	void Initialize(const Vector3& position, const Vector3& rotate);

	// 更新
	void Update();

	ViewProjection& GetViewCamera() { return viewProjection_; }

	WorldTransform& GetWorldMatrix() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

};
