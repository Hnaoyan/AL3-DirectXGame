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
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	// デバッグカメラの解放
	delete debugCamera_;
	delete railCamera_;
	// 天球モデル
	//delete skydome_;
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
	railCamera_->Initialize(Vector3{0, 0, -100.0f}, Vector3{0, 0, 0});
	// 自キャラ
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);
	player_->SetParent(&railCamera_->GetWorldMatrix());
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球
	//skydome_ = new Skydome();
	//skydome_->Initialize(modelSkydome_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//debugCamera_->SetFarZ(2.0f);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

	} else {
		// ビュープロジェクション行列の更新と転送
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewCamera().matView;
		viewProjection_.matProjection = railCamera_->GetViewCamera().matProjection;
		viewProjection_.TransferMatrix();
	}


	player_->Update(viewProjection_);

	this->deathTimer++;
	if (this->deathTimer == 185) {
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(model_, this, Vector3{float((count * 20) - 40), 1.0f, 100.0f});
		newEnemy->SetGameScene(this);
		newEnemy->SetPlayer(player_);
		enemys_.push_back(newEnemy);
		this->deathTimer = 0;
		if (count == 5) {
			count = 0;
		}
	}


	//enemy_->Update();
	// 死んでいるか判断
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	// 敵の更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	
	// 敵の弾が死んでるか判断
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//　敵弾の更新
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}

	
	// 天球
	//skydome_->Update();

	this->CheckAllCollision();
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
	player_->DrawUI();

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
	//skydome_->Draw(viewProjection_);

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();

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
#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
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
	for (Enemy* enemy : enemys_) {
		// 敵キャラの座標
		posA = enemy->GetWolrdPosition();
		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 弾の座標
			posB = bullet->GetWorldPosition();

			float distance = {
			    powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2) + powf(posA.z - posB.z, 2)};
			float radius = enemy->radius + bullet->radius;
			// 交差判定
			if (distance <= radius) {
				// 敵キャラの衝突時のコールバック
				enemy->OnCollision();
				// 自弾の衝突時のコールバック
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾の座標
	for (PlayerBullet* playerBullet_ : playerBullets) {
		posA = playerBullet_->GetWorldPosition();
		// 総当たり
		for (EnemyBullet* enemyBullet_ : enemyBullets_) {
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);

}