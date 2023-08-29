#include "FollowCamera.h"
#include "Vector3Utils.h"
#include "Matrix.h"
#include <Input.h>
#include "MathCalc.h"
#include <algorithm>
#include <ImGuiManager.h>

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void FollowCamera::Update() 
{
	//t += 0.01f;
	//t = std::clamp(t, 0.0f, 1.0f);

	// 追従対象がいれば
	if (target_) {
		interTarget_ = MathCalc::Lerp(interTarget_, target_->translation_, 0.01f);
	}

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		float rotateSpeed = 0.02f;
	
		destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
		//viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
	}

	viewProjection_.rotation_.y =
	    MathCalc::LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.5f);

	// 追従対象からのオフセット
	Vector3 offset = SetOffset();
	// 座標をコピーしてオフセット文ずらす
	viewProjection_.translation_ = Add(target_->translation_, offset);

	// ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	ImGui::Begin("camera");
	ImGui::Text(" 回転 : %f : %f", destinationAngleY_, viewProjection_.rotation_.y);	
	ImGui::Text(" %f : %f", interTarget_.x, interTarget_.z);
	ImGui::End();

}

void FollowCamera::Reset() 
{
	// 追従対象がいれば
	if (target_) {
		// 追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	
	}

}

Vector3 FollowCamera::SetOffset() const 
{
	// カメラまでのオフセット
	Vector3 offset = defaultOffset;
	// 回転行列の合成
	Matrix4x4 rotateMatrix = Matrix::Multiply(
	    Matrix::MakeRotateXMatrix(viewProjection_.rotation_.x),
	    Matrix::Multiply(
	        Matrix::MakeRotateYMatrix(viewProjection_.rotation_.y),
	        Matrix::MakeRotateZMatrix(viewProjection_.rotation_.z)));
	offset = Matrix::Transform(offset, rotateMatrix);
	return offset;
}
