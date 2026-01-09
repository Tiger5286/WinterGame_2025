#include "GameObject.h"
#include "../Utility/Collider.h"
#include "../Systems/Map.h"
#include <cassert>

namespace
{
	// 重力加速度
	const Vector2 kGravity = { 0.0f,1.0f };
	// 最大落下速度
	constexpr float kMaxFallSpeed = 15.0f;
	// マップチップの1辺の長さ
	constexpr int kMapChipSize = 16;
	// 描画スケール
	constexpr float kDrawScale = 3.0f;
}

GameObject::GameObject():
	_isGround(false)
{
	// 何もしない
}

GameObject::~GameObject()
{
	// 何もしない
}


HitDirection GameObject::MapCollision(Map& map)
{
	HitDirection ans;
	SetAllHitDir(ans, false);
	_pos.x += _vel.x;
	_pCollider->SetPosToBox(_pos);
	Vector2 hitChipPos;
	if (map.IsCollision(_pCollider, hitChipPos))	// xだけ移動した後に当たった = 横から当たった
	{
		Vector2 dist = _pCollider->GetPos() - hitChipPos;
		// 横から当たった場合
		if (dist.x > 0)
		{
			//	プレイヤーは当たったマップチップの右側にいる
			_pos.x = hitChipPos.x + _pCollider->GetSize().x / 2 + (kMapChipSize * kDrawScale) / 2;
			ans.left = true;
		}
		else
		{
			//	プレイヤーは当たったマップチップの左側にいる
			_pos.x = hitChipPos.x - _pCollider->GetSize().x / 2 - (kMapChipSize * kDrawScale) / 2;
			ans.right = true;
		}
		_vel.x = 0.0f;
		_pCollider->SetPosToBox(_pos);
		ans.any = true;
	}
	_pos.y += _vel.y;
	_pCollider->SetPosToBox(_pos);
	if (map.IsCollision(_pCollider, hitChipPos))	// yだけ移動した後に当たった = 縦から当たった
	{
		Vector2 dist = _pCollider->GetPos() - hitChipPos;
		// 縦から当たった場合
		if (dist.y > 0)
		{
			//	プレイヤーは当たったマップチップの下側にいる
			_pos.y = hitChipPos.y + _pCollider->GetSize().y + (kMapChipSize * kDrawScale) / 2;
			ans.up = true;
		}
		else
		{
			//	プレイヤーは当たったマップチップの上側にいる
			_pos.y = hitChipPos.y - (kMapChipSize * kDrawScale) / 2;
			ans.down = true;
			_isGround = true;
		}
		_vel.y = 0.0f;
		_pCollider->SetPosToBox(_pos);
		ans.any = true;
	}
	return ans;
}

void GameObject::ChangeAnim(Animation anim)
{
	if (_nowAnim != anim)
	{
		_nowAnim = anim;
	}
}

Vector2 GameObject::ChipPosToGamePos(Vector2 mapChipPos)
{
	Vector2 ans;
	ans.x = mapChipPos.x * kMapChipSize * kDrawScale + (kMapChipSize * kDrawScale) / 2;
	ans.y = mapChipPos.y * kMapChipSize * kDrawScale + (kMapChipSize * kDrawScale) / 2;
	return ans;
}

Vector2 GameObject::GamePosToChipPos(const Vector2& pos)
{
	Vector2 ans;
	ans.x = pos.x / (kMapChipSize * kDrawScale);
	ans.y = pos.y / (kMapChipSize * kDrawScale);
	return ans;
}


Vector2 GameObject::GetColliderPos() const
{
	return _pCollider->GetPos();
}

void GameObject::Gravity()
{
	_vel += kGravity;
	if (_vel.y > kMaxFallSpeed)
	{
		_vel.y = kMaxFallSpeed;
	}
}