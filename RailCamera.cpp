#include "RailCamera.h"
#include "Matrix.h"

void RailCamera::Initialize(const Vector3& position) { 
	// ワールドトランスフォームの初期設定
	worldTransform.translation_ = position;


	// ビュープロジェクションの初期化
	viewProjection->Initialize();
}

void RailCamera::Update() { 
	viewProjection->matView = MakeInverse(worldTransform.matWorld_);

}