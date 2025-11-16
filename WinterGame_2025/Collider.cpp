#include "Collider.h"

Collider::Collider(Vector2 pos, float radius) :
	_isEnabled(true),
	_pos(pos),
	_radius(radius),
	_size{ 0.0f,0.0f }
{
}

Collider::Collider(Vector2 pos, Vector2 size):
	_isEnabled(true),
	_pos(pos),
	_size(size),
	_radius(0.0f)
{
}

Collider::~Collider()
{
}