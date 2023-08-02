#include "Player.h"
#include<cassert>
#include "ImGuiManager.h"
#include "MathCalc.h"
#include <Windows.h>
#include "Vector.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GlobalVariables.h"

void Player::Initialize(const std::vector<Model*>& models) {
	// モデル読み込み
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// 初期化
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();

	// 親子関係　座標移動
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	worldTransformWeapon_.parent_ = &worldTransformBase_;

	worldTransformHead_.translation_ = {0, 1.8f, 0};
	worldTransformL_arm_.translation_ = {0.4f, 1.5f, -0.15f};
	worldTransformR_arm_.translation_ = {-0.4f, 1.5f, -0.15f};
	worldTransformWeapon_.translation_ = {0.0f, -0.4f, -0.8f};
	// 浮遊ギミック初期化
	InitializeFloatingGimmick();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Test", 90);

	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);

}

void Player::Update() 
{
	ApplyGlobalVariables();
	//XINPUT_STATE joyState;

	////if (!Input::GetInstance()->GetJoystickState())

	//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
	//	if (behavior_ != Behavior::kAttack) {
	//		behaviorRequest_ = Behavior::kAttack;
	//	}
	//}
		
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		//GlobalVariables::GetInstance()->SaveFile("Player");
		if (behavior_ != Behavior::kAttack) {
			behaviorRequest_ = Behavior::kAttack;
		}

	}
	

	if (behaviorRequest_) {
		// 行動変更
		behavior_ = behaviorRequest_.value();
		// それぞれの初期化
		switch (behavior_) {
		case Player::Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		// リクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Player::Behavior::kRoot:
		// 行動（基本）
		BehaviorRootUpdate();

		break;
	case Player::Behavior::kAttack:
		// 行動（攻撃）
		BehaviorAttackUpdate();

		break;
	}

	// ベースの行列計算
	worldTransformBase_.UpdateMatrix();

	// 転送
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

}

void Player::Draw(ViewProjection& viewProjection) 
{
	models_[BODY]->Draw(worldTransformBody_, viewProjection);
	models_[HEAD]->Draw(worldTransformHead_, viewProjection);
	models_[L_ARM]->Draw(worldTransformL_arm_, viewProjection);
	models_[R_ARM]->Draw(worldTransformR_arm_, viewProjection);
	if (behavior_ == Behavior::kAttack) {
		models_[WEAPON]->Draw(worldTransformWeapon_, viewProjection);
	}	
}

void Player::InitializeFloatingGimmick() 
{ 
	floatingParameter_ = 0.0f; 
}

void Player::UpdateFloatingGimmick() 
{
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(M_PI) / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(M_PI));

	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingWidth;

	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * floatingWidth;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * floatingWidth;

}

void Player::BehaviorRootUpdate() 
{
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed};

		// 移動量に速さを反映
		move = Scaler(MathCalc::Normalize(move), speed);

		worldTransformBase_.translation_ += move;
		worldTransformBase_.rotation_.y = std::atan2f(move.x, move.z);
		float length = sqrtf(move.x * move.x + move.z * move.z);
		worldTransformBase_.rotation_.x = std::atan2f(-move.y, length);
	}

	// 浮遊ギミック更新
	//UpdateFloatingGimmick();

}

void Player::BehaviorAttackUpdate() {
	switch (state_) {
	case Player::Attack::kDown:
		worldTransformWeapon_.rotation_.x += 0.02f;
		worldTransformL_arm_.rotation_.x += 0.02f;
		worldTransformR_arm_.rotation_.x += 0.02f;
		if (worldTransformWeapon_.rotation_.x > 1.5f) {
			state_ = Attack::kStop;
		}
		break;
	case Player::Attack::kStop:
		attackStanTime_++;

		if (attackStanTime_ == 60) {
			state_ = Attack::kDown;
			behaviorRequest_ = Behavior::kRoot;
		}
		break;
	}

}

void Player::BehaviorRootInitialize() {
	worldTransformL_arm_.rotation_ = {};
	worldTransformR_arm_.rotation_ = {};
}

void Player::BehaviorAttackInitialize() { 
	worldTransformL_arm_.rotation_.x = 3.0f;
	worldTransformR_arm_.rotation_.x = 3.0f;
	worldTransformWeapon_.rotation_ = {};
	state_ = Attack::kDown;
	attackStanTime_ = 0;
}

void Player::ApplyGlobalVariables() 
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ =
	    globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ =
	    globalVariables->GetVector3Value(groupName, "ArmR Translation");

}
