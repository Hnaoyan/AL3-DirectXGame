#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) 
{ 
	// モデル割り当て
	models_ = models;
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void BaseCharacter::Update() 
{
	// 行列更新
	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) 
{
	// モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
				
	}

}
