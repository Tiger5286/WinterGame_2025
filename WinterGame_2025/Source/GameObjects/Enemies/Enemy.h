#pragma once
#include "../GameObject.h"
#include <vector>

enum class EnemyType
{
	Normal,
	Boss,
};

class Player;
class EffectManager;
class SceneManager;
class Bullet;

class Enemy : public GameObject
{
public:
	Enemy(int hp, int score, std::shared_ptr<Player> pPlayer,std::shared_ptr<EffectManager> pEffectManager,SceneManager& sceneManager);
	virtual ~Enemy();

	virtual void Init() override = 0;
	virtual void Update(Map& map) override = 0;
	/// <summary>
	/// 状況関係なく毎フレーム呼ばれる更新処理(画面外に行っても呼ばれ続ける)
	/// </summary>
	virtual void UpdateAnytime();
	void BaseDraw(const Vector2& offset);
	virtual void Draw(Vector2 offset) override = 0;

	virtual EnemyType GetEnemyType() const = 0;

	int GetHp() const { return _hp; }
	virtual void TakeDamage(int damage, Bullet& bullet);
	virtual bool GetIsAlive() const;
	int GetScore() const { return _kScore; }
	void SetIsHitChargeShot(bool isHit) { _isHitChargeShot = isHit; }
	bool GetIsHitChargeShot()const { return _isHitChargeShot; }
	const Vector2& GetDrawPos() const { return _pos + _drawOffset; }
protected:
	void BaseUpdate();

protected:
	int _hp;
	const int _kScore;
	
	Vector2 _drawOffset;

	int _damageFrame;
	bool _isHitChargeShot;
	std::shared_ptr<EffectManager> _pEffectManager;
	std::shared_ptr<Player> _pPlayer;
	SceneManager& _sceneManager;
};

