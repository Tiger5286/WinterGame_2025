#include "ExplosionUpward.h"

namespace
{
	constexpr int kGraphSize = 128;
	constexpr float kDrawScale = 2.0f;

	constexpr int kIdleAnimNum = 12;
	constexpr int kOneAnimFrame = 4;

	constexpr int kPosYOffset = 50;
}

ExplosionUpward::ExplosionUpward(int handle, Vector2 pos):
	_handle(handle),
	Effect({ pos.x,pos.y - kGraphSize / 2 * kDrawScale + kPosYOffset })	// ó«Ç¢ä¥Ç∂ÇÃà íuÇ…Ç»ÇÈÇÊÇ§YÇí≤êÆ
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kIdleAnimNum, kOneAnimFrame, kDrawScale, false);
}
