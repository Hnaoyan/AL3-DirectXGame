#include "Player.h"
#include<cassert>
#include "ImGuiManager.h"
#include "MathCalc.h"
#include <Windows.h>
#include "Vector.h"

void Player::Initialize(Model* modelHead, Model* modelBody, Model* modelL_arm, Model* modelR_arm) {
	// NULLチェック
	assert(modelHead);
	assert(modelBody);
	assert(modelL_arm);
	assert(modelR_arm);

	// モデル読み込み
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	// 初期化
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	
	// 親子関係　座標移動
	worldTransformHead_.translation_ = {0, 1.8f, 0};
	worldTransformL_arm_.translation_ = {0.4f, 1.5f, 0};
	worldTransformR_arm_.translation_ = {-0.4f, 1.5f, 0};

}

void Player::Update() 
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

	// 親子関係作成
	// 体
	worldTransformBody_ = worldTransformBase_;
	// 頭
	worldTransformHead_.matWorld_ =
	    Matrix::Multiply(worldTransformBody_.matWorld_, worldTransformHead_.matWorld_);
	// 左
	worldTransformL_arm_.matWorld_ =
	    Matrix::Multiply(worldTransformBody_.matWorld_, worldTransformL_arm_.matWorld_);
	// 右
	worldTransformR_arm_.matWorld_ =
	    Matrix::Multiply(worldTransformBody_.matWorld_, worldTransformR_arm_.matWorld_);

	// 転送
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

}

void Player::Draw(ViewProjection& viewProjection) 
{
	// 3Dモデルを描画
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}
