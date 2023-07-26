﻿#include "Player.h"
#include<cassert>
#include "ImGuiManager.h"
#include "MathCalc.h"
#include <Windows.h>
#include "Vector.h"
#define _USE_MATH_DEFINES
#include <math.h>

void Player::Initialize(Model* modelHead, Model* modelBody, Model* modelL_arm, Model* modelR_arm) {
	// NULLチェック
	assert(modelHead);
	assert(modelBody);
	assert(modelL_arm);
	assert(modelR_arm);

	// モデル読み込み
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	// 初期化
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	
	// 親子関係　座標移動
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;


	worldTransformHead_.translation_ = {0, 1.8f, 0};
	worldTransformL_arm_.translation_ = {0.4f, 1.5f, -0.15f};
	worldTransformR_arm_.translation_ = {-0.4f, 1.5f, -0.15f};

	// 浮遊ギミック初期化
	InitializeFloatingGimmick();
}

void Player::Update() 
{
	// 浮遊ギミック更新
	UpdateFloatingGimmick();

	XINPUT_STATE joyState;
	
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed};

		// 移動量に速さを反映
		move = Scaler(MathCalc::Normalize(move), speed);

		worldTransformBase_.translation_ += move;
		worldTransformBase_.rotation_.y = std::atan2f(move.x, move.z);
		float length = sqrtf(move.x * move.x + move.z * move.z);
		worldTransformBase_.rotation_.x = std::atan2f(-move.y, length);

	}
	// ベースの行列計算
	worldTransformBase_.UpdateMatrix();

	// 転送
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

}

void Player::Draw(ViewProjection& viewProjection) 
{
	// 3Dモデルを描画
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { 
	floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() 
{
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(M_PI) / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(M_PI));

	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingWidth;

	worldTransformL_arm_.rotation_.z = std::sin(floatingParameter_) * floatingWidth;
	worldTransformR_arm_.rotation_.z = std::sin(-floatingParameter_) * floatingWidth;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head", &worldTransformHead_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmL", &worldTransformL_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR", &worldTransformR_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderInt("period", &period, 0, 120);
	ImGui::SliderFloat("floatWid", &floatingWidth, 0.0f, 1.0f);

	ImGui::End();

}
