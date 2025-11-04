#include "GameObject.h"

namespace
{
	const Vector2 GRAVITY = { 0.0f,1.0f };
	constexpr float MAX_FALL_SPEED = 15.0f;
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}


void GameObject::ChangeAnim(Animation anim)
{
	if (_nowAnim != anim)
	{
		_nowAnim = anim;
	}
}


void GameObject::Gravity()
{
	_vel += GRAVITY;
	if (_vel.y > MAX_FALL_SPEED)
	{
		_vel.y = MAX_FALL_SPEED;
	}
}

std::shared_ptr<Collider> GameObject::GetCollider()
{
	return _collider;
}
