#include "Player.h"
#include<cassert>
#include "ImGuiManager.h"
#include"Vector.h"

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL Check
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	// SingleInstance
	this->input_ = Input::GetInstance();
}

void Player::Rotate() {
	// RotateSpeed
	const float kRotSpeed = 0.02f;

	// Update
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Update() {
	// BulletDelete
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// MoveMaxMin
	// limitPos
	const float kMoveLimitsX = 15;
	const float kMoveLimitsY = 20;

	this->worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitsX);
	this->worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitsX);
	this->worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitsY);
	this->worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitsY);

	// scale
	Matrix4x4 scale = MakeScaleMatrix(this->worldTransform_.scale_);
	// AllRotate
	Matrix4x4 rotate = Multiply(
	    MakeRotateXMatrix(worldTransform_.rotation_.x),
	    Multiply(MakeRotateYMatrix(worldTransform_.rotation_.y), MakeRotateZMatrix(worldTransform_.rotation_.z)));
	// translate
	Matrix4x4 translate = MakeTranslateMatrix(worldTransform_.translation_);
	// affine
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// Matrix to Transform
	worldTransform_.TransferMatrix();

	// CharacterMoveVector
	Vector3 move = {0, 0, 0};

	// CharacterMoveSpeed
	const float kCharacterSpeed = 0.2f;

	// PressDirectChangeVector(LeftRight)
	if (this->input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (this->input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// TopBottom
	if (this->input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (this->input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// PositionMove
	MoveVector(worldTransform_.translation_, move);
	Rotate();
	Attack();

	// UpdateBullet
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	
	//DebugText
	float inputFloat3[3] = {
	worldTransform_.translation_.x, worldTransform_.translation_.y,
	worldTransform_.translation_.z};
	ImGui::Begin("pos");
	ImGui::InputFloat3("float3", inputFloat3);
	ImGui::SliderFloat3("position", inputFloat3, -10.0f, 10.0f);
	ImGui::End();

	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3DModel
	model_->Draw(worldTransform_, viewProjection, this->textureHandle_);
	// PositionDraw

	// BulletDraw
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)){

		// BulletSpeed
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// BulletRotate
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// BulletGeneration
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, this->worldTransform_.translation_, velocity);

		// 
		bullets_.push_back(newBullet);
	}
}