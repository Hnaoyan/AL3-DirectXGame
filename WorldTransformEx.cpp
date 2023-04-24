#include"WorldTransform.h"
#include"Matrix.h"

void WorldTransform::UpdateMatrix() {
	// Scale,Rotate,Translate
	this->matWorld_ = MakeAffineMatrix(this->scale_, this->rotation_, this->translation_);
	// 
	TransferMatrix();
}

