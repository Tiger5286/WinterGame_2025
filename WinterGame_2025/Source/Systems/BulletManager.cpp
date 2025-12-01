#include "BulletManager.h"
#include "Dxlib.h"
#include <cassert>
#include "../GameObjects/Bullet.h"

#include "Camera.h"

namespace
{
	constexpr int kBulletNum = 20;
}

BulletManager::BulletManager()
{
	_shotH = LoadGraph("data/Player/Shot.png");
	assert(_shotH != -1);
	_chargeShotH = LoadGraph("data/Player/ChargeShot.png");
	assert(_chargeShotH != -1);

	_pBullets.resize(kBulletNum);
	for (auto& bullet : _pBullets)
	{
		bullet = std::make_shared<Bullet>(_shotH, _chargeShotH);
	}
}

BulletManager::~BulletManager()
{
	DeleteGraph(_shotH);
	DeleteGraph(_chargeShotH);
}

void BulletManager::Update(Map& map, Vector2 cameraPos, std::vector<std::shared_ptr<Enemy>> pEnemies)
{
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->Update(map, cameraPos, pEnemies);
		}
	}
}

void BulletManager::Draw(Vector2 offset)
{
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->Draw(offset);
		}
	}
}
