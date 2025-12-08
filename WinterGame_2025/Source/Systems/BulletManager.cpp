#include "BulletManager.h"
#include "Dxlib.h"
#include <cassert>
#include "../GameObjects/Bullet.h"
#include "../GameObjects/FallBall.h"

#include "Camera.h"

namespace
{
	constexpr int kBulletNum = 20;
	constexpr int kFallBallNum = 10;
}

BulletManager::BulletManager(Vector2 mapSize):
	_kMapSize(mapSize)
{
	_shotH = LoadGraph("data/Player/Shot.png");
	assert(_shotH != -1);
	_chargeShotH = LoadGraph("data/Player/ChargeShot.png");
	assert(_chargeShotH != -1);
	_fallBallH = LoadGraph("data/Enemys/BouncingBall.png");
	assert(_fallBallH != -1);

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
	DeleteGraph(_fallBallH);
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

	for (auto& ball : _pFallBalls)
	{
		if (ball->GetIsAlive())
		{
			ball->Update(map);
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

	for (auto& ball : _pFallBalls)
	{
		if (ball->GetIsAlive())
		{
			ball->Draw(offset);
		}
	}
}

void BulletManager::Shot(BulletType type, Vector2 shotPos, bool isTurn)
{
	for (auto& bullet : _pBullets)
	{	// ‹ó‚¢‚Ä‚¢‚é’e‚ð’T‚µ‚Ä”­ŽË
		if (!bullet->GetAlive())
		{
			bullet->Shot(type, shotPos, isTurn);
			break;
		}
	}
}

void BulletManager::ShotFallBall(Vector2 pos)
{
	for (auto& ball : _pFallBalls)
	{
		if (!ball->GetIsAlive())
		{
			ball->Shot(pos);
			break;
		}
	}
}

void BulletManager::SetPlayer(std::shared_ptr<Player> pPlayer)
{
	_pPlayer = pPlayer;
	_pFallBalls.resize(kFallBallNum);
	for (auto& fallBall : _pFallBalls)
	{
		fallBall = std::make_shared<FallBall>(_pPlayer, _kMapSize, _fallBallH);
	}
}
