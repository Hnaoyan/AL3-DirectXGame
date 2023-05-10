#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	// 3Dモデルデータの解放
	delete model_;

	// 自キャラの解放
	delete player_;
	// 敵キャラの解放
	delete enemy_;
	// デバッグカメラの解放
	delete debugCamera_;
	delete railCamera_;
	// 天球モデル
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	// モデルデータの生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize(Vector3{0, 0, -50.0f}, Vector3{0, 0, 0});
	// 自キャラ
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);
	// 敵キャラ
	enemy_ = new Enemy();
	enemy_->Initialize(model_);
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//debugCamera_->SetFarZ(2.0f);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() { 
	player_->Update();
	debugCamera_->Update();
	railCamera_->Update();
	if (enemy_ != NULL) {
		enemy_->Update();
	}
	// 天球
	skydome_->Update();

	this->CheckAllCollision();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
		
	} else {
			// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 自キャラの描画
	player_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	if (enemy_ != NULL) {
		enemy_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float distance = {
		    powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2)};

		float radius = player_->radius + bullet->radius;

		// 球と球の交差判定
		if (distance <= radius) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWolrdPosition();
	// 敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 弾の座標
		posB = bullet->GetWorldPosition();

		float distance = {
		    powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2)};
		float radius = enemy_->radius + bullet->radius;
		// 交差判定
		if (distance <= radius) {
			// 敵キャラの衝突時のコールバック
			enemy_->OnCollision();
			// 自弾の衝突時のコールバック
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾の座標
	for (PlayerBullet* playerBullet_ : playerBullets) {
		posA = playerBullet_->GetWorldPosition();
		// 総当たり
		for (EnemyBullet* enemyBullet_ : enemyBullets) {
			posB = enemyBullet_->GetWorldPosition();
			float distance = {
			    powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2)};

			float radius = enemyBullet_->radius + playerBullet_->radius;
			// 交差判定
			if (distance <= radius) {
				// 敵キャラの衝突時のコールバック
				playerBullet_->OnCollision();
				// 自弾の衝突時のコールバック
				enemyBullet_->OnCollision();
			}
		}
	}


#pragma endregion
}