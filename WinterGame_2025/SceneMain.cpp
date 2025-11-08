#include "SceneMain.h"
#include "DxLib.h"
#include "Application.h"
#include "input.h"
#include "Player.h"
#include "Bullet.h"
#include "WalkEnemy.h"
#include "FlyEnemy.h"
#include "SceneManager.h"
#include "DebugScene.h"
#include <cassert>
#include "Game.h"
#include "Map.h"
#include "Camera.h"

namespace
{
	constexpr int BULLET_NUM = 15;
}

SceneMain::SceneMain(SceneManager& manager) :
	SceneBase(manager),
	_frameCount(0)
{
	/*画像の読み込み*/
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
	_flyEnemyH = LoadGraph("data/Enemy/FlyEnemy.png");
	assert(_flyEnemyH != -1);
	_mapChipH = LoadGraph("data/Map/MapChip.png");
	assert(_mapChipH != -1);

	// オブジェクトの生成
	// プレイヤー
	_pPlayer = std::make_shared<Player>();
	_pPlayer->SetHandle(_playerH,_chargeParticleH);

	// 弾
	_pBullets.resize(BULLET_NUM);
	for (auto& bullet : _pBullets)
	{
		bullet = std::make_shared<Bullet>();
		bullet->SetHandle(_playerShotH,_chargeShotH);
	}

	// 敵
	_pEnemys.push_back(std::make_shared<WalkEnemy>(_walkEnemyH, _pPlayer));
	_pEnemys.push_back(std::make_shared<WalkEnemy>(_walkEnemyH, _pPlayer));
	_pEnemys[0]->SetPos({ 300.0f,100.0f });
	std::shared_ptr<WalkEnemy> tempEnemy = std::dynamic_pointer_cast<WalkEnemy>(_pEnemys[0]);
	tempEnemy->SetState(WalkEnemyState::Move);
	_pEnemys[1]->SetPos({ 500.0f,100.0f });
	_pEnemys.push_back(std::make_shared<FlyEnemy>(_flyEnemyH,_pPlayer));
	_pEnemys[2]->SetPos({ 400.0f,600.0f });

	// マップ
	_pMap = std::make_shared<Map>(_mapChipH);

	// カメラ
	_pCamera = std::make_shared<Camera>(_pMap->GetStageWidth());
}

SceneMain::~SceneMain()
{
	DeleteGraph(_playerH);
	DeleteGraph(_playerShotH);
	DeleteGraph(_chargeShotH);
	DeleteGraph(_chargeParticleH);
	DeleteGraph(_walkEnemyH);
	DeleteGraph(_flyEnemyH);
	DeleteGraph(_mapChipH);
}

void SceneMain::Init()
{
	_pPlayer->Init();
	for (auto& bullet : _pBullets)
	{
		bullet->Init();
	}
	for (auto& enemy : _pEnemys)
	{
		enemy->Init();
	}
}

void SceneMain::Update(Input input)
{
	_frameCount++;

	// プレイヤー制御
	_pPlayer->SetContext(input,_pBullets);
	_pPlayer->Update(*_pMap);
	_pCamera->Update(_pPlayer->GetPos().x);

	// 敵制御
	for (auto& enemy : _pEnemys)
	{
		if (enemy != nullptr)
		{
			enemy->Update();
			// 体力が0以下になったら消す
			if (enemy->GetHp() <= 0)
			{
				enemy->Delete();
				// 死んだ敵をvectorから削除する(あんま意味わからん)
				_pEnemys.erase(
					std::remove_if(_pEnemys.begin(), _pEnemys.end(),
						[](const std::shared_ptr<Enemy>& enemy) {
							return !enemy->GetIsAlive();
						}),
					_pEnemys.end()
				);
			}
		}
	}

	// 弾制御
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->SetContext(_pEnemys);
			bullet->Update(_pCamera->GetPos());
		}
	}

	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
}

void SceneMain::Draw()
{
	_pMap->Draw(_pCamera->GetDrawOffset());
	//DrawBox(0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT, 0xffffff, true);
	for (auto& enemy : _pEnemys)
	{
		if (enemy != nullptr)
		{
			enemy->Draw(_pCamera->GetDrawOffset());
		}
	}
	_pPlayer->Draw(_pCamera->GetDrawOffset());
	for (auto& bullet : _pBullets)
	{
		bullet->Draw(_pCamera->GetDrawOffset());
	}

	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", _frameCount);
}