#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const Vector3& position,const Vector3& rotate) { 
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// SingleInstance
	this->input_ = Input::GetInstance();

	// ビュープロジェクションの初期化
	// farZの値変更

	viewProjection_.Initialize();

}

void RailCamera::Update() { 

	std::vector<Vector3> controlPoints_;
	controlPoints_ = {
	    {0,  0,  0},
        {10, 10, 0},
        {10, 15, 0},
        {10, 15, 0},
        {20, 0,  0},
        {30, 0,  0},
	};

	//worldTransform_.translation_.z = worldTransform_.translation_.z - 0.1f;
	//worldTransform_.rotation_.y = worldTransform_.rotation_.y + 0.01f;

	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= 0.005f;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += 0.005f;
	}

	XINPUT_STATE joyState;
	
	// ゲームパッド未接続なら何もせずに抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	

	if (joyState.Gamepad.bLeftTrigger) {
		worldTransform_.rotation_.y -= 0.005f;
	}
	if (joyState.Gamepad.bRightTrigger) {
		worldTransform_.rotation_.y += 0.005f;
	}
	

	// カメラ座標を表示
	ImGui::Begin("Camera");
	float inputFloatPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	float inputFloatRot[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z};
	ImGui::SliderFloat3("sliderFloat3", inputFloatPos, -100.0f, 100.0f);
	ImGui::SliderFloat3("rotate", inputFloatRot, -100.0f, 100.0f);
	ImGui::End();
	worldTransform_.translation_.x = inputFloatPos[0];
	worldTransform_.translation_.y = inputFloatPos[1];
	worldTransform_.translation_.z = inputFloatPos[2];
	worldTransform_.rotation_.x = inputFloatRot[0];
	worldTransform_.rotation_.y = inputFloatRot[1];
	worldTransform_.rotation_.z = inputFloatRot[2];

	
	//  アフィン変換
	worldTransform_.matWorld_ = Matrix::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Matrix::MakeInverse(worldTransform_.matWorld_);
	// ビュープロジェクションを転送
	viewProjection_.TransferMatrix();
}