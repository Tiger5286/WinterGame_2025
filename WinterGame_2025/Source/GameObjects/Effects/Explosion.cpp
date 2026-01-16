#include "Explosion.h"

namespace
{
	constexpr int kGraphSize = 48;
	constexpr float kDrawScale = 3.0f;
	constexpr float kDrawScaleSmall = 1.5f;
	constexpr float kDrawScaleBig = 6.0f;
	constexpr float kDrawScaleHuge = 12.0f;

	constexpr int kAnimNum = 10;
	constexpr int kOneAnimFrame = 4;
	constexpr int kOneAnimFrameSmall = 2;
	constexpr int kOneAnimFrameBig = 8;
	constexpr int kOneAnimFrameHuge = 12;
}

Explosion::Explosion(int handle,Vector2 pos,ExplosionType type):
	_handle(handle),
	Effect(pos)
{
	switch (type)
	{
	case ExplosionType::Normal:
		_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale, false);
		break;
	case ExplosionType::Small:
		_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrameSmall, kDrawScaleSmall, false);
		break;
	case ExplosionType::Big:
		_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrameBig, kDrawScaleBig, false);
		break;
	case ExplosionType::Huge:
		_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrameHuge, kDrawScaleHuge, false);
		break;
	default:
		break;
	}
	
}