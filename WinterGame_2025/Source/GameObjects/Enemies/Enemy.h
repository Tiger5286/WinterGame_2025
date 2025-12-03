#pragma once
#include "../GameObject.h"
#include <vector>

class Player;
class EffectManager;
class SceneManager;

class Enemy : public GameObject
{
public:
	Enemy(int hp, int score, std::shared_ptr<Player> pPlayer,std::shared_ptr<EffectManager> pEffectManager,SceneManager& sceneManager);
	virtual ~Enemy();

	virtual void Init() override = 0;
	virtual void Update(Map& map) override = 0;
	virtual void Draw(Vector2 offset) override = 0;

	int GetHp() const { return _hp; }
	virtual void TakeDamage(int damage);
	bool GetIsAlive() const;
	int GetScore() const { return _kScore; }
	void SetIsHitChargeShot(bool isHit) { _isHitChargeShot = isHit; }
	bool GetIsHitChargeShot()const { return _isHitChargeShot; }
protected:
	void BaseUpdate();

protected:
	int _hp;
	const int _kScore;

	int _damageFrame;
	bool _isHitChargeShot;
	std::shared_ptr<EffectManager> _pEffectManager;
	std::shared_ptr<Player> _pPlayer;
	SceneManager& _sceneManager;
};

