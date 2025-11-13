#pragma once
#include "Geometry.h"
#include "Animation.h"
#include <memory>

class Input;
class Collider;
class Map;

// どっちから当たっているかを表す構造体
struct HitDirection
{
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	void SetAll(bool isHit)
	{
		up = isHit;
		down = isHit;
		left = isHit;
		right = isHit;
	}
};

enum class HitDirectionX
{
	None,
	Left,
	Right
};

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() = 0;
	virtual void Update(Map& map) = 0;
	virtual void Draw(Vector2 offset) = 0;

	/// <summary>
	/// マップとの当たり判定をし、押し戻す
	/// </summary>
	/// <param name="map">マップ</param>
	/// <returns>true:当たっている/false:当たっていない</returns>
	bool MapCollision(Map& map);
	bool MapCollision(Map& map, HitDirection& hitDir);	// 当たった方向を取得するバージョン
	bool MapCollision(Map& map, HitDirectionX& hitDirX);	// 左右の当たった方向を取得するバージョン
	bool MapCollision(Map& map, bool& isHitUp);	// 上に当たったかどうかを取得するバージョン

	void ChangeAnim(Animation anim);

	/// <summary>
	/// マップチップ座標をゲーム座標に変換する
	/// </summary>
	/// <param name="mapChipPos">マップチップ座標</param>
	/// <returns>ゲーム内座標</returns>
	static Vector2 MapChipPosToGamePos(Vector2 mapChipPos);

	void SetPos(Vector2 pos) { _pos = pos; }
	Vector2 GetPos() { return _pos; }

	void SetPosFromMapChip(Vector2 mapChipPos)
	{
		_pos = MapChipPosToGamePos(mapChipPos);
	}

	std::shared_ptr<Collider> GetCollider() { return _collider; }

protected:
	void Gravity();

protected:
	Vector2 _pos;
	Vector2 _vel;
	std::shared_ptr<Collider> _collider;
	Animation _nowAnim;
	bool _isGround;		// 接地フラグ
	HitDirection _hitDir;
};