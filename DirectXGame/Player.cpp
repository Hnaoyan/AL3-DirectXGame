#include "Player.h"
#include<cassert>
#include "ImGuiManager.h"
#include "MathCalc.h"
#include <Windows.h>
#include "Vector.h"


Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
	
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL Check
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 30.0f};
	
	// SingleInstance
	this->input_ = Input::GetInstance();
	// 3Dレティクルの初期化
	this->offsetModel_ = model;
	worldTransform3DReticle_.Initialize();
	offsetTexture_ = TextureManager::Load("offset.png");

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle,Vector2(0,0),Vector4(1,1,1,1),Vector2(0.5f,0.5f));
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

void Player::Update(ViewProjection& viewProjection) {
	// 弾の消去
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
	Matrix4x4 scale = Matrix::MakeScaleMatrix(this->worldTransform_.scale_);
	// AllRotate
	Matrix4x4 rotate = Matrix::Multiply(
	    Matrix::MakeRotateXMatrix(worldTransform_.rotation_.x),
	    Matrix::Multiply(
	        Matrix::MakeRotateYMatrix(worldTransform_.rotation_.y),
	        Matrix::MakeRotateZMatrix(worldTransform_.rotation_.z)));
	// translate
	Matrix4x4 translate = Matrix::MakeTranslateMatrix(worldTransform_.translation_);

	// Matrix to Transform
	worldTransform_.UpdateMatrix();

	// CharacterMoveVector
	Vector3 move = {0, 0, 0};

	// CharacterMoveSpeed
	const float kCharacterSpeed = 0.5f;

	

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

	// プレイヤー移動
	worldTransform_.translation_ = worldTransform_.translation_ + move;
	Rotate();
	Attack();

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	
	// デバッグテキスト
	float inputFloat3[3] = {
	worldTransform_.translation_.x, worldTransform_.translation_.y,
	worldTransform_.translation_.z};
	float float3[3] = {
	    worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y,
	    worldTransform3DReticle_.translation_.z};
	ImGui::Begin("pos");
	ImGui::InputFloat3("float3", inputFloat3);
	ImGui::SliderFloat3("position", inputFloat3, -50.0f, 50.0f);
	ImGui::InputFloat3("offset", float3);
	ImGui::End();

	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];
	{
		// 自機のワールド座標から3Dレティクルのワールド座標を計算
		// 自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 20.0f;
		// 自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};
		// 自機のワールド行列の回転を反映
		offset = MathCalc::TransformNormal(offset, worldTransform_.matWorld_);
		// ベクトルの長さを整える
		offset = {
		    MathCalc::Normalize(offset).x * kDistancePlayerTo3DReticle,
		    MathCalc::Normalize(offset).y * kDistancePlayerTo3DReticle,
		    MathCalc::Normalize(offset).z * kDistancePlayerTo3DReticle};
		// 座標設定
		worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;
		worldTransform3DReticle_.UpdateMatrix();
	}
	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = Matrix::GetWorldPosition(worldTransform3DReticle_.matWorld_);

		// ビューポート行列
		Matrix4x4 matViewport =
		    Matrix::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// ビューポート行列
		Matrix4x4 matViewProjectionViewport = Matrix::Multiply(
		    Matrix::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

		// ワールド→スクリーン座標変換
		positionReticle = Matrix::Transform(positionReticle, matViewProjectionViewport);

		// スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}
	{
		// ビューポート行列
		Matrix4x4 matViewport =
		    Matrix::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		POINT mousePosition;
		// マウス座標（スクリーン座標）を取得する
		GetCursorPos(&mousePosition);

		// クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);
		Vector2 mouse = {float(mousePosition.x), float(mousePosition.y)};
		sprite2DReticle_->SetPosition(mouse);

		// ビュープロジェクションビューポート合成行列
		Matrix4x4 matVPV = Matrix::Multiply(
		    Matrix::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
		// 合成行列の逆行列を計算
		Matrix4x4 matInverseVPV = Matrix::MakeInverse(matVPV);
	
		// スクリーン座標
		Vector3 posNear = Vector3(WinApp::kWindowWidth, WinApp::kWindowHeight, 0);
		Vector3 posFar = Vector3(WinApp::kWindowWidth, WinApp::kWindowHeight, 1);
	
		// スクリーン座標系からワールド座標系へ
		posNear = Matrix::Transform(posNear, matInverseVPV);
		posFar = Matrix::Transform(posFar, matInverseVPV);

		// マウスレイの方向
		Vector3 mouseDirection = Vector::Subtract(posNear, posFar);
		mouseDirection = MathCalc::Normalize(mouseDirection);
		// カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 50.0f;
		worldTransform3DReticle_.translation_ = Vector3(Vector::Add(posNear, mouseDirection));
		//worldTransform3DReticle_.translation_.x =
		//    Vector3(Vector::Subtract(posNear, mouseDirection)).x + kDistanceTestObject;

		//worldTransform3DReticle_.translation_.y =
		//    Vector3(Vector::Subtract(posNear, mouseDirection)).y + kDistanceTestObject;
		worldTransform3DReticle_.translation_.x += kDistanceTestObject;
		worldTransform3DReticle_.translation_.y += kDistanceTestObject;

		worldTransform3DReticle_.translation_.z += kDistanceTestObject;


		worldTransform3DReticle_.UpdateMatrix();

	}
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3DModel
	model_->Draw(worldTransform_, viewProjection, this->textureHandle_);
	// レティクルオフセット
	//offsetModel_->Draw(worldTransform3DReticle_, viewProjection, offsetTexture_);


	// PositionDraw

	// BulletDraw
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw(); 

}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos = Matrix::GetWorldPosition(worldTransform_.matWorld_);
	
	return worldPos;
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)){

		// 弾速
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 弾回転
		//velocity = MathCalc::TransformNormal(velocity, worldTransform_.matWorld_);

		velocity = worldTransform3DReticle_.translation_;
		velocity = velocity - worldTransform_.translation_;
		velocity = {
		    MathCalc::Normalize(velocity).x * kBulletSpeed,
		    MathCalc::Normalize(velocity).y * kBulletSpeed,
		    MathCalc::Normalize(velocity).z * kBulletSpeed};
		Vector3 worldPos = GetWorldPosition();

		// 弾生成
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldPos, velocity);

		// 
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {

}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
