#include "SmallDust.h"

namespace
{
	constexpr int kGraphSize = 32;
	constexpr float kDrawScale = 2.0f;

	constexpr int kAnimNum = 8;
	constexpr int kOneAnimFrame = 2;
}

SmallDust::SmallDust(int handle, Vector2 pos):
	_handle(handle),
	Effect(pos)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale, false);
}