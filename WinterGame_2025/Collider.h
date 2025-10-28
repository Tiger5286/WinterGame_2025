#pragma once
#include "Geometry.h"

/// <summary>
/// “–‚½‚è”»’è‚ð‚·‚éŠî’êƒNƒ‰ƒX
/// </summary>
class Collider
{
public:
	Collider(Vector2 pos,float radius);
	Collider(Vector2 pos, Vector2 size);
	virtual ~Collider();

	virtual bool CheckCollision(const Collider& other) const = 0;
	virtual void Draw() const = 0;

	enum class Type
	{
		Circle,
		Box
	};
	virtual Type GetType() const = 0;
	void SetPos(const Vector2 pos);

	Vector2 GetPos() const;
	float GetRadius() const;
	Vector2 GetSize() const;
	
protected:
	Vector2 _pos;
	float _radius;
	Vector2 _size;
};