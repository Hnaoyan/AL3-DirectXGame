#include "FollowCamera.h"
#include "Vector3Utils.h"

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

		// 座標をコピーしてオフセット文ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
		
	}

	// ビュー行列の更新
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

}
