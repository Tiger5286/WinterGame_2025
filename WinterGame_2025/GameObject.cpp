#include "GameObject.h"
#include "Collider.h"
#include "Map.h"

namespace
{
	const Vector2 GRAVITY = { 0.0f,1.0f };
	constexpr float MAX_FALL_SPEED = 15.0f;
	constexpr float DRAW_SCALE = 3.0f;
}

GameObject::GameObject():
	_isGround(false)
{
}

GameObject::~GameObject()
{
}


void GameObject::MapCollision(Map& map)
{
	_pos.x += _vel.x;
	_collider->SetPos(_pos);
	Vector2 hitChipPos;
	if (map.IsCollision(_collider, hitChipPos))	// xだけ移動した後に当たった = 横から当たった
	{
		Vector2 dist = _collider->GetPos() - hitChipPos;
		// 横から当たった場合
		if (dist.x > 0)
		{
			//	プレイヤーは当たったマップチップの右側にいる
			_pos.x = hitChipPos.x + _collider->GetSize().x / 2 + (16 * DRAW_SCALE) / 2;
		}
		else
		{
			//	プレイヤーは当たったマップチップの左側にいる
			_pos.x = hitChipPos.x - _collider->GetSize().x / 2 - (16 * DRAW_SCALE) / 2;
		}
		_vel.x = 0.0f;
		_collider->SetPos(_pos);
	}
	_pos.y += _vel.y;
	_collider->SetPos(_pos);
	if (map.IsCollision(_collider, hitChipPos))	// yだけ移動した後に当たった = 縦から当たった
	{
		Vector2 dist = _collider->GetPos() - hitChipPos;
		// 縦から当たった場合
		if (dist.y > 0)
		{
			//	プレイヤーは当たったマップチップの下側にいる
			_pos.y = hitChipPos.y + _collider->GetSize().y + (16 * DRAW_SCALE) / 2;
		}
		else
		{
			//	プレイヤーは当たったマップチップの上側にいる
			_pos.y = hitChipPos.y - (16 * DRAW_SCALE) / 2;
			_isGround = true;
		}
		_vel.y = 0.0f;
		_collider->SetPos(_pos);
	}
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