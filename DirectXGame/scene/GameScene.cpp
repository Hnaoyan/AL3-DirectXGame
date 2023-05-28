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
	railCamera_->Initialize(Vector3{0, 0, -100.0f}, Vector3{0, 0, 0});
	// 自キャラ
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);
	player_->SetParent(&railCamera_->GetWorldMatrix());
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

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
	skydome_->Update();

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
	skydome_->Draw(viewProjection_);

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
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// コライダー
	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_);
	for (Enemy* enemy : enemys_) {
		colliders_.push_back(enemy);
	}
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		colliders_.push_back(enemyBullet);
	}
	for (PlayerBullet* playerBullet : playerBullets) {
		colliders_.push_back(playerBullet);
	}

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		//itrA = colliders_

		// イテレータBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
		

		}

	}

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
		CheckCollisionPair(bullet, player_);
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemys_) {
		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			CheckCollisionPair(enemy, bullet);
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾の座標
	for (PlayerBullet* playerBullet_ : playerBullets) {
		// 総当たり
		for (EnemyBullet* enemyBullet_ : enemyBullets_) {
			CheckCollisionPair(playerBullet_, enemyBullet_);
		}
	}


#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);

}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 pointA = colliderA->GetWorldPosition();
	Vector3 pointB = colliderB->GetWorldPosition();
	float distance = {
	    powf(pointA.x - pointB.x, 2) + powf(pointA.y - pointB.y, 2) + powf(pointA.z - pointB.z, 2)};
	float radius = colliderA->GetterRad() + colliderB->GetterRad();
	// 交差判定
	if (distance <= radius) {
		// ColliderAの衝突時のコールバック
		colliderA->OnCollision();
		// ColliderBの衝突時のコールバック
		colliderB->OnCollision();
	}
}
