#include "CircleCollider.h"
#include "Dxlib.h"
#include <cmath>

CircleCollider::CircleCollider(Vector2 pos, float radius) :
	Collider(pos,radius)
{
}

CircleCollider::~CircleCollider()
{
}

bool CircleCollider::CheckCollision(const Collider& other) const
{
	if (other.GetType() == Type::Circle)
	{
		float hitDis = _radius + other.GetRadius();
		//float disX = _pos.x - other

	}
	else if (other.GetType() == Type::Box)
	{

	}
	return false;
}

void CircleCollider::Draw() const
{
	DrawCircle(_pos.x, _pos.y, _radius, 0xff0000, false);
}