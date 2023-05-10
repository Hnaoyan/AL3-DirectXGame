#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> f, uint32_t time) { 
	time_ = time;
	
}

void TimedCall::Update() { 
	if (isComp_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isComp_ = true;
		// コールバック関数呼び出し
		this->f();
	}

}