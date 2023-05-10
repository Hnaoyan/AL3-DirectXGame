#pragma once
#include <functional>

/// <summary>
/// 時限発動
/// </summary>
class TimedCall 
{
public:
	// コンストラクタ
	TimedCall(std::function<void(void)> f, uint32_t time);
	void Update();
	// 完了ならtrueを返す
	bool IsFinished() { return isComp_; }

private:
	// コールバック
	std::function<void(void)> f_;
	// 残り時間
	uint32_t time_ = 0;
	// 完了フラグ
	bool isComp_ = false;
};
