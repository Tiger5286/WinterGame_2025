#pragma once
#include "GameObject.h"
#include "../Utility/Geometry.h"
#include <vector>

class Enemy;
class Gimmick;

enum class BulletType
{
	NormalShot,
	ChargeShot,
};

class Bullet : public GameObject
{
public:
	Bullet(int shotH, int chargeShotH);
	~Bullet();

	void Init() override;
	void Update(Map& map) override;
	void Update(Map& map, Vector2 cameraPos, std::vector<std::shared_ptr<Enemy>> pEnemys, std::vector<std::shared_ptr<Gimmick>> pGimmicks);
	void Draw(Vector2 offset) override;

	void Shot(BulletType type, Vector2 shotPos, bool isTurn);

	void Hit();
	void Reflect();

	bool GetAlive() const { return _isAlive; }

	BulletType GetType() const { return _type; }
private:

	int _shotH;
	int _chargeShotH;

	Animation _shotAnim;
	Animation _shotImpactAnim;
	Animation _chargeShotAnim;
	Animation _chargeShotImpactAnim;

	//std::vector<std::shared_ptr<Enemy>> _pEnemys;

	BulletType _type;
	bool _isReflected = false;
	bool _isImpact;
	bool _isAlive;
	bool _isTurn;
};

