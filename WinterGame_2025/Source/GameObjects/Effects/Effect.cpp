#include "Effect.h"

Effect::Effect(Vector2 pos)
{
	_pos = pos;
}

Effect::~Effect()
{
}

void Effect::Init()
{
}

void Effect::Update(Map& map)
{
}

void Effect::Update()
{
	_nowAnim.Update();
	if (_nowAnim.GetIsEnd())
	{
		_isAlive = false;
	}
}

void Effect::Draw(Vector2 offset)
{
	_nowAnim.Draw(_pos - offset, false);
}