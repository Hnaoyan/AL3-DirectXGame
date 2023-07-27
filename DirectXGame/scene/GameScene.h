#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Skydome.h"
#include "Ground.h"
#include "DebugCamera.h"
#include "FollowCamera.h"
#include <memory>
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

private: // ポインタ
	// 3Dモデルデータ
	std::unique_ptr<Model> charModel_; 
	// 天球モデル
	std::unique_ptr<Model> skydomeModel_;
	// 地面モデル
	std::unique_ptr<Model> groundModel_;
	// 敵キャラモデル
	std::unique_ptr<Model> enemyModel_;
	// パーツモデル
	std::unique_ptr<Model> modelHead_;
	std::unique_ptr<Model> modelBody_;
	std::unique_ptr<Model> modelR_arm_;
	std::unique_ptr<Model> modelL_arm_;
	// 武器モデル
	std::unique_ptr<Model> modelWeapon_;

	// 自キャラ
	std::unique_ptr<Player> player_;
	

	// 敵キャラ
	std::unique_ptr<Enemy> enemy_;

	// 天球
	std::unique_ptr<Skydome> skydome_;

	// 地面
	std::unique_ptr<Ground> ground_;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
