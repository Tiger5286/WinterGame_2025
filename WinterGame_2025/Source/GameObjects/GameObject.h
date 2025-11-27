#pragma once
#include "../Geometry.h"
#include "../Animation.h"
#include <memory>

class Input;
class Collider;
class Map;

// どっちから当たっているかを表す構造体
struct HitDirection
{
	bool any = false;	// どこかしら当たっている
	bool up = false;	// 上に当たった
	bool down = false;	// 下に当たった
	bool left = false;	// 左に当たった
	bool right = false;	// 右に当たった
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
	HitDirection MapCollision(Map& map);
	static void SetAllHitDir(HitDirection& hitDir, bool isHit)	// 当たり判定構造体の全ての方向をまとめて設定する関数
	{
		hitDir.any = isHit;
		hitDir.up = isHit;
		hitDir.down = isHit;
		hitDir.left = isHit;
		hitDir.right = isHit;
	}

	void ChangeAnim(Animation anim);

	/// <summary>
	/// マップチップ座標をゲーム座標に変換する
	/// </summary>
	/// <param name="mapChipPos">マップチップ座標</param>
	/// <returns>ゲーム内座標</returns>
	static Vector2 ChipPosToGamePos(Vector2 mapChipPos);

	void SetPos(Vector2 pos) { _pos = pos; }
	Vector2 GetPos() { return _pos; }

	void SetPosFromChipPos(Vector2 mapChipPos)
	{
		_pos = ChipPosToGamePos(mapChipPos);
	}

	std::shared_ptr<Collider> GetCollider() { return _pCollider; }

protected:
	void Gravity();

protected:
	Vector2 _pos;
	Vector2 _vel;
	std::shared_ptr<Collider> _pCollider;
	Animation _nowAnim;
	bool _isGround;		// 接地フラグ
	HitDirection _hitDir;
};