#include "SceneMain.h"
#include "DxLib.h"
#include "Application.h"
#include "input.h"
#include "Player.h"
#include "Bullet.h"
#include "WalkEnemy.h"
#include <cassert>

namespace
{
	constexpr int BULLET_NUM = 15;
}

SceneMain::SceneMain() :
	_frameCount(0)
{
	_playerH = LoadGraph("data/Player/Player.png");
	assert(_playerH != -1);
	_playerShotH = LoadGraph("data/Player/Shot.png");
	assert(_playerShotH != -1);
	_chargeShotH = LoadGraph("data/Player/ChargeShot.png");
	assert(_chargeShotH != -1);
	_chargeParticleH = LoadGraph("data/Player/ChargeParticle.png");
	assert(_chargeParticleH != -1);
	_walkEnemyH = LoadGraph("data/Enemy/WalkEnemy.png");
	assert(_walkEnemyH != -1);

	_pPlayer = std::make_shared<Player>();
	_pPlayer->SetHandle(_playerH);
	_pPlayer->SetOtherHandle(_chargeParticleH);

	_pBullets.resize(BULLET_NUM);
	for (auto& bullet : _pBullets)
	{
		bullet = std::make_shared<Bullet>();
		bullet->SetHandle(_playerShotH);
	}

	_pWalkEnemy = std::make_shared<WalkEnemy>();
	_pWalkEnemy->SetHandle(_walkEnemyH);
}

SceneMain::~SceneMain()
{
	DeleteGraph(_playerH);
	DeleteGraph(_playerShotH);
}

void SceneMain::Init()
{
	_pPlayer->Init();
	for (auto& bullet : _pBullets)
	{
		bullet->Init();
	}
	_pWalkEnemy->Init();
}

void SceneMain::Update(Input input)
{
	_frameCount++;

	// ƒvƒŒƒCƒ„[§Œä
	_pPlayer->SetContext(input,_pBullets);
	_pPlayer->Update();

	// ’e§Œä
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->Update();
		}
	}

	// “G§Œä
	if (_pWalkEnemy != nullptr)
	{
		_pWalkEnemy->SetContext(_pBullets);
		_pWalkEnemy->Update();
		if (_pWalkEnemy->GetHp() <= 0)
		{
			_pWalkEnemy = nullptr;
		}
	}
}

void SceneMain::Draw()
{
	if (_pWalkEnemy != nullptr)
	{
		_pWalkEnemy->Draw();
	}
	_pPlayer->Draw();
	for (auto& bullet : _pBullets)
	{
		bullet->Draw();
	}

	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", _frameCount);
}