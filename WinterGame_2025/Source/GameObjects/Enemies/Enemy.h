#pragma once
#include "../GameObject.h"
#include <vector>

class Player;
class EffectManager;
class SceneManager;

class Enemy : public GameObject
{
public:
	Enemy(int hp, std::shared_ptr<Player> pPlayer,std::shared_ptr<EffectManager> pEffectManager,SceneManager& sceneManager);
	virtual ~Enemy();

	virtual void Init() override = 0;
	virtual void Update(Map& map) override = 0;
	virtual void Draw(Vector2 offset) override = 0;

	void SetHp(int hp) { _hp = hp; }
	int GetHp() const { return _hp; }
	virtual void TakeDamage(int damage);
	bool GetIsAlive() const;
	void SetIsHitChargeShot(bool isHit) { _isHitChargeShot = isHit; }
	bool GetIsHitChargeShot()const { return _isHitChargeShot; }
protected:
	void BaseUpdate();

protected:
	int _hp;
	int _damageFrame;
	bool _isHitChargeShot;
	std::shared_ptr<EffectManager> _pEffectManager;
	std::shared_ptr<Player> _pPlayer;
	SceneManager& _sceneManager;
};

