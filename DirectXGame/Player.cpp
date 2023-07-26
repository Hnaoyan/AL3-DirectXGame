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

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHead_.translation_ = {0, 1.8f, 0};
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
	worldTransformHead_.matWorld_ =
	    Matrix::Multiply(worldTransformBase_.matWorld_, worldTransformHead_.matWorld_);

	// 転送
	worldTransformBase_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();

}

void Player::Draw(ViewProjection& viewProjection) 
{
	// 3Dモデルを描画
	modelBody_->Draw(worldTransformBase_, viewProjection);
	//modelHead_->Draw(worldTransformHead_, viewProjection);
}
