#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
public:
	BoxCollider(Vector2 pos, Vector2 size);
	~BoxCollider();

	bool CheckCollision(const std::shared_ptr<Collider> other) const override;
	void Draw(Vector2 offset) const override;
	Type GetType() const override { return Type::Box; }
};