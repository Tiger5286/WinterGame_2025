#include "ItemGet.h"

namespace
{
	constexpr int kGraphSize = 32;
	constexpr float kDrawScale = 3.0f;

	constexpr int kIdleAnimNum = 5;
	constexpr int kOneAnimFrame = 6;
}

ItemGet::ItemGet(int handle, Vector2 pos):
	_handle(handle),
	Effect(pos)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kIdleAnimNum, kOneAnimFrame, kDrawScale, false);
}