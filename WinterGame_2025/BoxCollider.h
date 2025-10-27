#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
public:
	BoxCollider(Vector2 pos, Vector2 size);
	~BoxCollider();

	bool CheckCollision(const Collider& other) const override;
	void Draw() const override;
	Type GetType() const override { return Type::Box; }
private:
	Vector2 _size;
};

