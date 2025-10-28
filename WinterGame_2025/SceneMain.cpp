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
	_walkEnemyH = LoadGraph("data/Enemy/WalkEnemy.png");
	assert(_walkEnemyH != -1);

	_pPlayer = std::make_shared<Player>();
	_pPlayer->SetHandle(_playerH);

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
	_pPlayer->SetContext(input,_pBullets);
	_pPlayer->Update();
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->Update();
		}
	}
	_pWalkEnemy->Update();
}

void SceneMain::Draw()
{
	_pWalkEnemy->Draw();
	_pPlayer->Draw();
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->Draw();
		}
	}

	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", _frameCount);
}