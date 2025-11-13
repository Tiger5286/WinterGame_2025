#pragma once
#include "Geometry.h"
#include <memory>

/// <summary>
/// 当たり判定をする基底クラス
/// </summary>
class Collider
{
public:
	Collider(Vector2 pos, float radius);
	Collider(Vector2 pos, Vector2 size);
	virtual ~Collider();

	virtual bool CheckCollision(const std::shared_ptr<Collider> other) const = 0;
	virtual void Draw(Vector2 offset) const = 0;

	enum class Type
	{
		Circle,
		Box
	};
	virtual Type GetType() const = 0;

	/// <summary>
	/// 指定した位置が中央下になるように当たり判定をセットする
	/// これはボックス型の当たり判定のみに有効で、サークル型に使うと普通に中心にセットされる
	/// </summary>
	/// <param name="pos">位置ベクトル</param>
	void SetPosToBox(const Vector2 pos) { _pos = { pos.x,pos.y - _size.y / 2 }; }
	void SetPos(const Vector2 pos) { _pos = pos; }
	Vector2 GetPos() const { return _pos; }

	float GetRadius() const { return _radius; }
	void SetRadius(float radius) { _radius = radius; }

	Vector2 GetSize() const { return _size; }
	
protected:
	Vector2 _pos;
	float _radius;
	Vector2 _size;
};