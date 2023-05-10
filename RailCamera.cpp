#include "RailCamera.h"
#include "Matrix.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const Vector3& position,const Vector3& rotate) { 
	// ワールドトランスフォームの初期設定
	worldTransform.translation_ = position;
	worldTransform.rotation_ = rotate;

	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

void RailCamera::Update() { 
	//worldTransform.translation_


	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection.matView = MakeInverse(worldTransform.matWorld_);
	// ビュープロジェクションを転送
	viewProjection.TransferMatrix();

	// カメラ座標を画面表示する処理
	ImGui::Begin("Camera");
	//ImGui::SliderFloat3("translate", );
	ImGui::End();
}