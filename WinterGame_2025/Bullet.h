#pragma once
#include "GameObject.h"
#include "Geometry.h"
#include "vector"

class Enemy;

enum class BulletType
{
	NormalShot,
	ChargeShot
};

class Bullet : public GameObject
{
public:
	Bullet(int shotH, int chargeShotH);
	~Bullet();

	void Init() override;
	void Update(Map& map) override;
	void Update(Map& map, Vector2 cameraPos);
	void Draw(Vector2 offset) override;

	void Shot(BulletType type, Vector2 shotPos, bool isTurn);

	void SetContext(std::vector<std::shared_ptr<Enemy>> pEnemys);

	void SetIsTurn(bool isTurn) { _isTurn = isTurn; }

	void SetAlive(bool isAlive) { _isAlive = isAlive; }
	bool GetAlive() const { return _isAlive; }

	void SetType(BulletType type) { _type = type; }
	BulletType GetType() const { return _type; }
private:
	int _shotH;
	int _chargeShotH;

	std::vector<std::shared_ptr<Enemy>> _pEnemys;

	BulletType _type;
	bool _isAlive;
	bool _isTurn;
};

