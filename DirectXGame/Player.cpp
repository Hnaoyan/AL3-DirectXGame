#include "Player.h"
#include<cassert>
#include "ImGuiManager.h"
#include "MathCalc.h"
#include <Windows.h>
#include "Vector.h"

void Player::Initialize(Model* model, uint32_t textureHandle) 
{
	// NULLチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

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

		worldTransform_.translation_ += move;
		worldTransform_.rotation_.y = std::atan2f(move.x, move.z);
		float length = sqrtf(move.x * move.x + move.z * move.z);
		worldTransform_.rotation_.x = std::atan2f(-move.y, length);

	}

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	//worldTransform_.TransferMatrix();


}

void Player::Draw(ViewProjection& viewProjection) 
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
