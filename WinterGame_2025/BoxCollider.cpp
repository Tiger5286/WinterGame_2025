#include "BoxCollider.h"
#include "Dxlib.h"

BoxCollider::BoxCollider(Vector2 pos, Vector2 size) :
	Collider(pos),
	_size(size)
{
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::CheckCollision(const Collider& other) const
{
	return false;
}

void BoxCollider::Draw() const
{
	DrawBox(_pos.x - _size.x / 2, _pos.y - _size.y, _pos.x + _size.x / 2, _pos.y, 0xff0000, false);
}
