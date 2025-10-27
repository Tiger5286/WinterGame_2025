#pragma once
#include "Geometry.h"

/// <summary>
/// “–‚½‚è”»’è‚ð‚·‚éŠî’êƒNƒ‰ƒX
/// </summary>
class Collider
{
public:
	Collider(Vector2 pos);
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
protected:
	Vector2 _pos;
};

