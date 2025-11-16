#pragma once
#include "Collider.h"
#include "Geometry.h"

/// <summary>
/// ‰~Œ`‚Ì“–‚½‚è”»’è‚ğ‚ÂƒNƒ‰ƒX
/// </summary>
class CircleCollider : public Collider
{
public:
	CircleCollider(Vector2 pos, float radius);
	~CircleCollider();

	bool CheckCollision(const std::shared_ptr<Collider> other) const override;
	void Draw(Vector2 offset) const override;

	Type GetType() const override { return Type::Circle; }
};