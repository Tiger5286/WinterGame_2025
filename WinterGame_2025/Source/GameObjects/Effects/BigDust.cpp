#include "BigDust.h"

namespace
{
	constexpr int kGraphSizeW = 48;
	constexpr int kGraphSizeH = 32;
	constexpr float kDrawScale = 2.5f;

	constexpr int kAnimNum = 10;
	constexpr int kOneAnimFrame = 2;
}

BigDust::BigDust(int handle, Vector2 pos):
	_handle(handle),
	Effect(pos)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSizeW, kGraphSizeH), kAnimNum, kOneAnimFrame, kDrawScale, false);
}