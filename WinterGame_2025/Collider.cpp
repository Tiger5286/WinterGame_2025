#include "Collider.h"

Collider::Collider(Vector2 pos):
	_pos(pos)
{
}

Collider::~Collider()
{
}

void Collider::SetPos(const Vector2 pos)
{
	_pos = pos;
}
