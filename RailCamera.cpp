#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const Vector3& position,const Vector3& rotate) { 
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;

	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

void RailCamera::Update() { 
	//worldTransform.translation_

	Matrix4x4 rotate = Multiply(
	    MakeRotateXMatrix(worldTransform_.rotation_.x),
	    Multiply(
	        MakeRotateYMatrix(worldTransform_.rotation_.y),
	        MakeRotateZMatrix(worldTransform_.rotation_.z)));
	// translate
	Matrix4x4 translate = MakeTranslateMatrix(worldTransform_.translation_);
	// affine
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// Matrix to Transform
	worldTransform_.TransferMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = MakeInverse(worldTransform_.matWorld_);
	// ビュープロジェクションを転送
	viewProjection_.TransferMatrix();

	// カメラ座標を画面表示する処理
	ImGui::Begin("Camera");
	//ImGui::SliderFloat3("translate", );
	ImGui::End();
}