#pragma once
#include "GameObject.h"
#include "Geometry.h"

enum class BulletType
{
	NormalShot,
	ChargeShot
};

class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Shot(BulletType type, Vector2 shotPos, bool isTurn);

	void SetHandle(int shotH,int chargeShotH);
	void SetType(BulletType type);

	void SetAlive(bool isAlive);
	bool GetAlive();

	void SetPos(Vector2 pos);
	void SetIsTurn(bool isTurn);
private:
	int _shotH;
	int _chargeShotH;

	BulletType _type;

	bool _isAlive;
	bool _isTurn;
};

