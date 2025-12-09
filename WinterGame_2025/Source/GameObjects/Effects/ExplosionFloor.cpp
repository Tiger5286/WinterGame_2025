#include "ExplosionFloor.h"

namespace
{
	constexpr int kGraphSizeW = 128;
	constexpr int kGraphSizeH = 80;
	const Vector2 kGraphSize = { kGraphSizeW, kGraphSizeH };
	constexpr float kDrawScale = 3.0f;

	constexpr int kAnimNum = 10;
	constexpr int kOneAnimFrame = 4;
}

ExplosionFloor::ExplosionFloor(int handle, Vector2 pos):
	_handle(handle),
	Effect({ pos.x,pos.y - kGraphSizeH / 2 * kDrawScale })	// è∞ÇÃçÇÇ≥Ç…çáÇÌÇπÇƒYç¿ïWÇí≤êÆ
{
	_nowAnim.Init(_handle, 0, kGraphSize, kAnimNum, kOneAnimFrame, kDrawScale, false);
}