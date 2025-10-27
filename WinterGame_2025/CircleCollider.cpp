#include "CircleCollider.h"
#include "Dxlib.h"

CircleCollider::CircleCollider(Vector2 pos, float radius) :
	Collider(pos),
	_radius(radius)
{
}

CircleCollider::~CircleCollider()
{
}

bool CircleCollider::CheckCollision(const Collider& other) const
{
	return false;
}

void CircleCollider::Draw() const
{
	DrawCircle(_pos.x, _pos.y, _radius, 0xff0000, false);
}