#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const Vector3& position,const Vector3& rotate) { 
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	addTranslation = {};
	addRotate = {};

	// ビュープロジェクションの初期化
	// farZの値変更

	viewProjection_.Initialize();

}

void RailCamera::Update() { 

	worldTransform_.translation_.x = worldTransform_.translation_.x + addTranslation.x;
	worldTransform_.translation_.y = worldTransform_.translation_.y + addTranslation.y;
	worldTransform_.translation_.z = worldTransform_.translation_.z + addTranslation.z;
	worldTransform_.rotation_.x = worldTransform_.rotation_.x + addRotate.x;
	worldTransform_.rotation_.y = worldTransform_.rotation_.y + addRotate.y;
	worldTransform_.rotation_.z = worldTransform_.rotation_.z + addRotate.z;

	// カメラ座標を表示
	ImGui::Begin("Camera");
	float inputFloatPos[3] = {addTranslation.x, addTranslation.y, addTranslation.z};
	float inputFloatRot[3] = {addRotate.x, addRotate.y, addRotate.z};
	ImGui::SliderFloat3("sliderFloat3", inputFloatPos, -100.0f, 100.0f);
	ImGui::SliderFloat3("rotate", inputFloatRot, -100.0f, 100.0f);
	ImGui::End();
	addTranslation.x = inputFloatPos[0];
	addTranslation.y = inputFloatPos[1];
	addTranslation.z = inputFloatPos[2];

	
	//  affine
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = MakeInverse(worldTransform_.matWorld_);
	// ビュープロジェクションを転送
	viewProjection_.TransferMatrix();
}