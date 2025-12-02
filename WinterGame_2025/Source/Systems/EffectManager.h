#pragma once
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"

class Effect;

enum class EffectType
{
	Explosion,
};

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	void Update();
	void Draw(Vector2 offset);
	void Create(Vector2 pos,EffectType type);

private:
	int _explosionH = -1;
	std::vector<std::shared_ptr<Effect>> _pEffects;
};