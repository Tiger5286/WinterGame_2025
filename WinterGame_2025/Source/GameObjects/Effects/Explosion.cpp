#include "Explosion.h"

namespace
{
	constexpr int kGraphSize = 48;
	constexpr float kDrawScale = 3.0f;

	constexpr int kAnimNum = 10;
	constexpr int kOneAnimFrame = 4;
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
		_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, 2, 1.5f , false);
		break;
	case ExplosionType::Big:
		_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, 8, 6.0f , false);
		break;
	default:
		break;
	}
	
}