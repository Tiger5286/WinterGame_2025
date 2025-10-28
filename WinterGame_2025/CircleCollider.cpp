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

bool CircleCollider::CheckCollision(const std::shared_ptr<Collider> other) const
{
	if (other->GetType() == Type::Circle)
	{
		// ‰~“¯Žm‚Ì“–‚½‚è”»’è
		float hitDis = _radius + other->GetRadius();
		float disX = _pos.x - other->GetPos().x;
		float disY = _pos.y - other->GetPos().y;
		float dis = sqrtf(disX * disX + disY * disY);
		if (dis < hitDis)
		{
			return true;
		}
	}
	else if (other->GetType() == Type::Box)
	{
		// ‰~‚Æ‹éŒ`‚Ì“–‚½‚è”»’è
		float hitDisX = _radius + other->GetSize().x;
		float hitDisY = _radius + other->GetSize().y;
		float disX = _pos.x - other->GetPos().x;
		float disY = _pos.y - other->GetPos().y;
		if ((disX < hitDisX) && (disY < hitDisY))
		{
			return true;
		}
	}
	return false;
}

void CircleCollider::Draw() const
{
	DrawCircle(_pos.x, _pos.y, _radius, 0xff0000, false);
}