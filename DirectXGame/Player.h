#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include"Matrix.h"
#include <list>
#include "Sprite.h"

class Player
{
public:

	Player(){};
	~Player(){};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(Model* modelHead,Model* modelBody,Model* modelL_arm,Model* modelR_arm);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);	

	/// <summary>
	/// Getter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	/// <summary>
	/// Viewのセッター
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// モデル
	Model* model_ = nullptr;

	Model* modelHead_ = nullptr;
	Model* modelBody_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
