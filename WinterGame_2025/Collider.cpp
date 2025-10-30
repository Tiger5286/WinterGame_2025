#include "Collider.h"

Collider::Collider(Vector2 pos, float radius) :
	_pos(pos),
	_radius(radius),
	_size{ 0.0f,0.0f }
{
}

Collider::Collider(Vector2 pos, Vector2 size):
	_pos(pos),
	_size(size),
	_radius(0.0f)
{
	_pos.y -= _size.y / 2;
}

Collider::~Collider()
{
}

void Collider::SetPos(const Vector2 pos)
{
	_pos = pos;
	_pos.y -= _size.y / 2;
}

Vector2 Collider::GetPos() const
{
	return _pos;
}

float Collider::GetRadius() const
{
	return _radius;
}

Vector2 Collider::GetSize() const
{
	return _size;
}
