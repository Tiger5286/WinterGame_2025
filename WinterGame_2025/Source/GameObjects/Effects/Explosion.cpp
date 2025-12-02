#include "Explosion.h"

namespace
{
	constexpr int kGraphSize = 48;
	constexpr float kDrawScale = 3.0f;

	constexpr int kAnimNum = 10;
	constexpr int kOneAnimFrame = 4;
}

Explosion::Explosion(int handle,Vector2 pos):
	_handle(handle),
	Effect(pos)
{
	_nowAnim.Init(_handle, 0, Vector2(kGraphSize, kGraphSize), kAnimNum, kOneAnimFrame, kDrawScale, false);
}

void Explosion::Update()
{
	Effect::Update();
}

void Explosion::Draw(Vector2 offset)
{
	Effect::Draw(offset);
}