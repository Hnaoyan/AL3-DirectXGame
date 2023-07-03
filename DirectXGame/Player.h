#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include"Matrix.h"
#include <list>
#include "Sprite.h"

class Player
{
public:

	Player(){};
	~Player(){};

	void Initialize(Model* model,uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);	

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
