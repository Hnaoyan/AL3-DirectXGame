#pragma once
#include "BaseCharacter.h"
#include "Matrix.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include <Input.h>
#include <list>
#include <optional>

class Player : public BaseCharacter {
public:
	Player(){};
	~Player(){};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	void Initialize(const std::vector<Model*>& models) override;

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
	WorldTransform* GetWorldTransform() { return &worldTransformBase_; }

	/// <summary>
	/// Viewのセッター
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

public:
	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();

	void BehaviorDashInitialize();

	void BehaviorDashUpdate();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private:
	// ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

private:
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	/// 浮遊の振幅<m>
	float floatingWidth = 0.25f;

	/// 浮遊移動のサイクル<frame>
	int period = 60;

	enum ModelPart {
		BODY,
		HEAD,
		L_ARM,
		R_ARM,
		WEAPON,
	};

	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kDash,	// ダッシュ中
	};

	enum class Attack {
		kDown,
		kStop,
	};

	struct WorkDash {
		// ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
	};

	// 状態
	Behavior behavior_ = Behavior::kRoot;

	Attack state_;

	WorkDash workDash_;

	int attackStanTime_ = 0;

	float angle = 0;

	float angleX = 0;

	// 次の動きのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};