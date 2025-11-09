#include "BoxCollider.h"
#include "Dxlib.h"
#include <cmath>

BoxCollider::BoxCollider(Vector2 pos, Vector2 size) :
	Collider(pos,size)
{
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::CheckCollision(const std::shared_ptr<Collider> other) const
{
	if (other->GetType() == Type::Circle)
	{
		// ‰~‚Æ‹éŒ`‚Ì“–‚½‚è”»’è
		float hitDisX = _size.x / 2 + other->GetRadius();
		float hitDisY = _size.y / 2 + other->GetRadius();
		float disX = _pos.x - other->GetPos().x;
		float disY = _pos.y - other->GetPos().y;
		if ((abs(disX) < hitDisX) && (abs(disY) < hitDisY))
		{
			return true;
		}
	}
	else if (other->GetType() == Type::Box)
	{
		// ‹éŒ`“¯Žm‚Ì“–‚½‚è”»’è
		float hitDisX = _size.x / 2 + other->GetSize().x / 2;
		float hitDisY = _size.y / 2 + other->GetSize().y / 2;
		float disX = _pos.x - other->GetPos().x;
		float disY = _pos.y - other->GetPos().y;
		if ((abs(disX) < hitDisX) && (abs(disY) < hitDisY))
		{
			return true;
		}
	}
	return false;
}

void BoxCollider::Draw(Vector2 offset) const
{
	DrawBox(_pos.x - _size.x / 2 - offset.x,
		_pos.y - _size.y / 2 - offset.y,
		_pos.x + _size.x / 2 - offset.x,
		_pos.y + _size.y / 2 - offset.y,
		0xff0000, false);
}