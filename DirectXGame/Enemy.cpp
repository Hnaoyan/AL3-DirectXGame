#include "Enemy.h"
#include <cmath>
#include <numbers>
#include "Matrix.h"

void Enemy::Initialize(Model* model) 
{
	// モデル読み込み
	model_ = model;
	// ワールド変換初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = {-5.0f, 0.0f, 20.0f};
	worldTransform_.scale_ = {0.8f, 0.8f, 0.8f};

	//worldTransform_.matWorld_

	velocity_ = {0, 0, 0.1f};
	

}

void Enemy::Update() 
{
	velocity_ = {0.0f, 0.0f, 0.1f};
	// 回転
	worldTransform_.rotation_.y += 0.01f;
	// 2πを超えたら戻す
	worldTransform_.rotation_.y =
	    (float)(std::fmod(worldTransform_.rotation_.y, 2.0f * std::numbers::pi));

	Matrix4x4 rotate = Matrix::MakeRotateYMatrix(worldTransform_.rotation_.y);

	velocity_ = Matrix::Transform(velocity_, rotate);

	worldTransform_.translation_ += velocity_;

	// 行列更新
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection);
}
