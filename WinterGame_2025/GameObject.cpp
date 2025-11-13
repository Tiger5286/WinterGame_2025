#include "GameObject.h"
#include "Collider.h"
#include "Map.h"
#include <cassert>

namespace
{
	const Vector2 GRAVITY = { 0.0f,1.0f };
	constexpr float MAX_FALL_SPEED = 15.0f;
	constexpr int MAP_CHIP_SIZE = 16;
	constexpr float DRAW_SCALE = 3.0f;
}

GameObject::GameObject():
	_isGround(false)
{
}

GameObject::~GameObject()
{
}


HitDirection GameObject::MapCollision(Map& map)
{
	HitDirection ans;
	SetAllHitDir(ans, false);
	_pos.x += _vel.x;
	_collider->SetPosToBox(_pos);
	Vector2 hitChipPos;
	if (map.IsCollision(_collider, hitChipPos))	// xだけ移動した後に当たった = 横から当たった
	{
		Vector2 dist = _collider->GetPos() - hitChipPos;
		// 横から当たった場合
		if (dist.x > 0)
		{
			//	プレイヤーは当たったマップチップの右側にいる
			_pos.x = hitChipPos.x + _collider->GetSize().x / 2 + (16 * DRAW_SCALE) / 2;
			ans.left = true;
		}
		else
		{
			//	プレイヤーは当たったマップチップの左側にいる
			_pos.x = hitChipPos.x - _collider->GetSize().x / 2 - (16 * DRAW_SCALE) / 2;
			ans.right = true;
		}
		_vel.x = 0.0f;
		_collider->SetPosToBox(_pos);
		ans.any = true;
	}
	_pos.y += _vel.y;
	_collider->SetPosToBox(_pos);
	if (map.IsCollision(_collider, hitChipPos))	// yだけ移動した後に当たった = 縦から当たった
	{
		Vector2 dist = _collider->GetPos() - hitChipPos;
		// 縦から当たった場合
		if (dist.y > 0)
		{
			//	プレイヤーは当たったマップチップの下側にいる
			_pos.y = hitChipPos.y + _collider->GetSize().y + (16 * DRAW_SCALE) / 2;
			ans.up = true;
		}
		else
		{
			//	プレイヤーは当たったマップチップの上側にいる
			_pos.y = hitChipPos.y - (16 * DRAW_SCALE) / 2;
			ans.down = true;
			_isGround = true;
		}
		_vel.y = 0.0f;
		_collider->SetPosToBox(_pos);
		ans.any = true;
	}
	return ans;
}

//bool GameObject::MapCollision(Map& map, HitDirection& hitDir)
//{
//	hitDir.SetAll(false);
//	bool collided = false;
//	Vector2 tempPos = _pos;
//	tempPos.x += _vel.x;
//	_collider->SetPosToBox(tempPos);
//	Vector2 hitChipPos;
//	if (map.IsCollision(_collider, hitChipPos))	// xだけ移動した後に当たった = 横から当たった
//	{
//		Vector2 dist = _collider->GetPos() - hitChipPos;
//		// 横から当たった場合
//		if (dist.x > 0)
//		{
//			//	プレイヤーは当たったマップチップの右側にいる
//			hitDir.left = true;
//		}
//		else
//		{
//			//	プレイヤーは当たったマップチップの左側にいる
//			hitDir.right = true;
//		}
//		_collider->SetPosToBox(tempPos);
//		collided = true;
//	}
//	tempPos.y += _vel.y;
//	_collider->SetPosToBox(tempPos);
//	if (map.IsCollision(_collider, hitChipPos))	// yだけ移動した後に当たった = 縦から当たった
//	{
//		Vector2 dist = _collider->GetPos() - hitChipPos;
//		// 縦から当たった場合
//		if (dist.y > 0)
//		{
//			//	プレイヤーは当たったマップチップの下側にいる
//			hitDir.up = true;
//		}
//		else
//		{
//			//	プレイヤーは当たったマップチップの上側にいる
//			hitDir.down = true;
//		}
//		collided = true;
//	}
//	_collider->SetPosToBox(_pos);
//	return collided;
//}
//
//bool GameObject::MapCollision(Map& map, HitDirectionX& HitDirX)
//{
//	bool collided = false;
//	_pos.x += _vel.x;
//	_collider->SetPosToBox(_pos);
//	HitDirX = HitDirectionX::None;
//	Vector2 hitChipPos;
//	if (map.IsCollision(_collider, hitChipPos))	// xだけ移動した後に当たった = 横から当たった
//	{
//		Vector2 dist = _collider->GetPos() - hitChipPos;
//		// 横から当たった場合
//		if (dist.x > 0)
//		{
//			//	プレイヤーは当たったマップチップの右側にいる(左から当たった)
//			_pos.x = hitChipPos.x + _collider->GetSize().x / 2 + (16 * DRAW_SCALE) / 2;
//			HitDirX = HitDirectionX::Left;
//		}
//		else
//		{
//			//	プレイヤーは当たったマップチップの左側にいる(右から当たった)
//			_pos.x = hitChipPos.x - _collider->GetSize().x / 2 - (16 * DRAW_SCALE) / 2;
//			HitDirX = HitDirectionX::Right;
//		}
//		_vel.x = 0.0f;
//		_collider->SetPosToBox(_pos);
//		collided = true;
//	}
//	_pos.y += _vel.y;
//	_collider->SetPosToBox(_pos);
//	if (map.IsCollision(_collider, hitChipPos))	// yだけ移動した後に当たった = 縦から当たった
//	{
//		Vector2 dist = _collider->GetPos() - hitChipPos;
//		// 縦から当たった場合
//		if (dist.y > 0)
//		{
//			//	プレイヤーは当たったマップチップの下側にいる(頭をぶつけた)
//			_pos.y = hitChipPos.y + _collider->GetSize().y + (16 * DRAW_SCALE) / 2;
//		}
//		else
//		{
//			//	プレイヤーは当たったマップチップの上側にいる(乗っている)
//			_pos.y = hitChipPos.y - (16 * DRAW_SCALE) / 2;
//			_isGround = true;
//		}
//		_vel.y = 0.0f;
//		_collider->SetPosToBox(_pos);
//		collided = true;
//	}
//	return collided;
//}
//
//bool GameObject::MapCollision(Map& map, bool& isHitUp)
//{
//	bool collided = false;
//	_pos.x += _vel.x;
//	_collider->SetPosToBox(_pos);
//	Vector2 hitChipPos;
//	isHitUp = false;
//	if (map.IsCollision(_collider, hitChipPos))	// xだけ移動した後に当たった = 横から当たった
//	{
//		Vector2 dist = _collider->GetPos() - hitChipPos;
//		// 横から当たった場合
//		if (dist.x > 0)
//		{
//			//	プレイヤーは当たったマップチップの右側にいる
//			_pos.x = hitChipPos.x + _collider->GetSize().x / 2 + (16 * DRAW_SCALE) / 2;
//		}
//		else
//		{
//			//	プレイヤーは当たったマップチップの左側にいる
//			_pos.x = hitChipPos.x - _collider->GetSize().x / 2 - (16 * DRAW_SCALE) / 2;
//		}
//		_vel.x = 0.0f;
//		_collider->SetPosToBox(_pos);
//		collided = true;
//	}
//	_pos.y += _vel.y;
//	_collider->SetPosToBox(_pos);
//	if (map.IsCollision(_collider, hitChipPos))	// yだけ移動した後に当たった = 縦から当たった
//	{
//		Vector2 dist = _collider->GetPos() - hitChipPos;
//		// 縦から当たった場合
//		if (dist.y > 0)
//		{
//			//	プレイヤーは当たったマップチップの下側にいる
//			_pos.y = hitChipPos.y + _collider->GetSize().y + (16 * DRAW_SCALE) / 2;
//			isHitUp = true;
//		}
//		else
//		{
//			//	プレイヤーは当たったマップチップの上側にいる
//			_pos.y = hitChipPos.y - (16 * DRAW_SCALE) / 2;
//			_isGround = true;
//		}
//		_vel.y = 0.0f;
//		_collider->SetPosToBox(_pos);
//		collided = true;
//	}
//	return collided;
//}

void GameObject::ChangeAnim(Animation anim)
{
	if (_nowAnim != anim)
	{
		_nowAnim = anim;
	}
}

Vector2 GameObject::MapChipPosToGamePos(Vector2 mapChipPos)
{
	Vector2 ans;
	ans.x = mapChipPos.x * MAP_CHIP_SIZE * DRAW_SCALE + (MAP_CHIP_SIZE * DRAW_SCALE) / 2;
	ans.y = mapChipPos.y * MAP_CHIP_SIZE * DRAW_SCALE + (MAP_CHIP_SIZE * DRAW_SCALE) / 2;
	return ans;
}


void GameObject::Gravity()
{
	_vel += GRAVITY;
	if (_vel.y > MAX_FALL_SPEED)
	{
		_vel.y = MAX_FALL_SPEED;
	}
}