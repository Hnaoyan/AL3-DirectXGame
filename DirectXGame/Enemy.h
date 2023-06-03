#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"TextureManager.h"
#include <Input.h>
#include <list>
#include"EnemyBullet.h"
#include"TimedCall.h"
#include "Collider.h"

class Player;

class GameScene;

enum class Phase {
	Approach,	// Near
	Leave,		// Leave
};

class Enemy : public Collider
{
public:
	// Initialize
	void Initialize(Model* model, GameScene* gameScene, Vector3 position);
	// Update
	void Update();
	// Draw
	void Draw(const ViewProjection& viewProjection);

	// Pattern
	void Approach();
	void Leave();

	// Collision
	void OnCollision() override;

	// ワールド座標を取得
	Vector3 GetWorldPosition() override;

	// Bullet
	void Fire();

	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	~Enemy();

	// 弾の発射間隔
	static const int kFireInterval = 80;

	/// <summary>
	/// 弾のリスト
	/// </summary>
	/// <returns>弾のリスト</returns>
	const std::list<EnemyBullet*>& GetBullets() { return enemyBullets_; }
	
	//　半径
	const float radius = 15.0f;

	// ゲームシーンのポインタを取得
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// 死亡フラグを返す
	bool IsDead() const { return isDead_; }

	//void FireTimeReset();

private:
	static void (Enemy::*spFuncTable[])();

	// Player
	Player* player_ = nullptr;

	// world
	WorldTransform worldTransform_;
	// Model
	Model* model_ = nullptr;
	// Texture
	uint32_t textureHandle_ = 0u;
	// Velocity
	Vector3 velocity_ = {};
	// Phase	
	Phase phase_ = Phase::Approach;
	// Bullet
	std::list<EnemyBullet*> enemyBullets_;
	// Keyboard
	Input* input_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	int32_t shootTimer = 0;

	// 死亡フラグ
	bool isDead_ = false;

	std::list<TimedCall*> timedCalls_;

//private:
//	// 衝突属性（自分）
//	uint32_t collisionAttribute_ = 0xffffffff;
//	// 衝突マスク（相手）
//	uint32_t collisionMask_ = 0xffffffff;
//
//public:
//	// 衝突属性（自分）を取得
//	uint32_t GetCollisionAttribute() override;
//	// 衝突属性（自分）を設定
//	void SetCollisionAttribute(uint32_t attribute) override;
//	// 衝突マスク（相手）を取得
//	uint32_t GetCollisionMask() override;
//	// 衝突マスク（相手）を設定
//	void SetCollisionMask(uint32_t mask) override;
};
