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
	void Initialize();

	// 更新
	void Update();

private:
	// ワールド変換データ
	WorldTransform worldTransform;

	// ビュープロジェクション
	ViewProjection viewProjection;

};
