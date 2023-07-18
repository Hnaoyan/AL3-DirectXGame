#include "FollowCamera.h"
#include "Vector3Utils.h"
#include "Matrix.h"
#include <Input.h>

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void FollowCamera::Update() 
{
	// 追従対象がいれば
	if (target_) {
		// カメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		Matrix4x4 rotateMatrix = Matrix::Multiply(
		    Matrix::MakeRotateXMatrix(viewProjection_.rotation_.x),
		    Matrix::Multiply(
		        Matrix::MakeRotateYMatrix(viewProjection_.rotation_.y),
		        Matrix::MakeRotateZMatrix(viewProjection_.rotation_.z)));
		offset = Matrix::Transform(offset, rotateMatrix);

		// 座標をコピーしてオフセット文ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
		
	}

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		float rotateSpeed = 0.02f;
	

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotateSpeed;
	}

	// ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

}
