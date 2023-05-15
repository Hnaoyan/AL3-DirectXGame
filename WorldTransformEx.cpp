#include"WorldTransform.h"
#include"Matrix.h"

void WorldTransform::UpdateMatrix() {
	// Scale,Rotate,Translate
	this->matWorld_ = MakeAffineMatrix(this->scale_, this->rotation_, this->translation_);

	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	// 定数バッファに転送する
	TransferMatrix();
}

