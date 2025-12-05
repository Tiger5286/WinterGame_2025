#include "EffectManager.h"
#include "Dxlib.h"
#include <cassert>

#include "../GameObjects/Effects/Effect.h"
#include "../GameObjects/Effects/Explosion.h"
#include "../GameObjects/Effects/ItemGet.h"
#include "../GameObjects/Effects/SmallDust.h"

EffectManager::EffectManager()
{
	_explosionH = LoadGraph("data/Effects/Explosion.png");
	assert(_explosionH != -1);
	_itemGetH = LoadGraph("data/Effects/ItemGet.png");
	assert(_itemGetH != -1);
	_smallDustH = LoadGraph("data/Effects/SmallDust.png");
	assert(_smallDustH != -1);
}

EffectManager::~EffectManager()
{
	DeleteGraph(_explosionH);
	DeleteGraph(_itemGetH);
}

void EffectManager::Update()
{
	for (auto& effect : _pEffects)
	{
		effect->Update();
	}

	_pEffects.erase(std::remove_if(_pEffects.begin(), _pEffects.end(), [](auto effect) {
		return !effect->GetIsAlive();
		}), _pEffects.end());
}

void EffectManager::Draw(Vector2 offset)
{
	for (auto& effect : _pEffects)
	{
		effect->Draw(offset);
	}
}

void EffectManager::Create(Vector2 pos,EffectType type)
{
	switch (type)
	{
	case EffectType::Explosion:
		_pEffects.push_back(std::make_shared<Explosion>(_explosionH, pos));
		break;
	case EffectType::ExplosionSmall:
		_pEffects.push_back(std::make_shared<Explosion>(_explosionH, pos, ExplosionType::Small));
		break;
	case EffectType::ExplosionBig:
		_pEffects.push_back(std::make_shared<Explosion>(_explosionH, pos, ExplosionType::Big));
		break;
	case EffectType::ItemGet:
		_pEffects.push_back(std::make_shared<ItemGet>(_itemGetH, pos));
		break;
	case EffectType::SmallDust:
		_pEffects.push_back(std::make_shared<SmallDust>(_smallDustH,pos));
		break;
	default:
		break;
	}
}
