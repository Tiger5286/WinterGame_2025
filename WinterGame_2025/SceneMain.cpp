#include "SceneMain.h"
#include "DxLib.h"
#include "Application.h"
#include "input.h"
#include "Player.h"
#include "Bullet.h"
#include "WalkEnemy.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include <cassert>

namespace
{
	constexpr int BULLET_NUM = 15;
}

SceneMain::SceneMain(SceneManager& manager) :
	SceneBase(manager),
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
	_pPlayer->SetHandle(_playerH,_chargeParticleH);

	_pBullets.resize(BULLET_NUM);
	for (auto& bullet : _pBullets)
	{
		bullet = std::make_shared<Bullet>();
		bullet->SetHandle(_playerShotH,_chargeShotH);
	}

	_pEnemys.push_back(std::make_shared<WalkEnemy>(_walkEnemyH));
	_pEnemys.push_back(std::make_shared<WalkEnemy>(_walkEnemyH));
	_pEnemys[0]->SetPos({ 300.0f,100.0f });
	_pEnemys[1]->SetPos({ 500.0f,100.0f });
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
	for (auto& enemy : _pEnemys)
	{
		enemy->Init();
	}
}

void SceneMain::Update(Input input)
{
	_frameCount++;

	// ÉvÉåÉCÉÑÅ[êßå‰
	_pPlayer->SetContext(input,_pBullets);
	_pPlayer->Update();

	// ìGêßå‰
	for (auto& enemy : _pEnemys)
	{
		if (enemy != nullptr)
		{
			enemy->Update();
			// ëÃóÕÇ™0à»â∫Ç…Ç»Ç¡ÇΩÇÁè¡Ç∑
			if (enemy->GetHp() <= 0)
			{
				enemy->Delete();
				// éÄÇÒÇæìGÇvectorÇ©ÇÁçÌèúÇ∑ÇÈ(à”ñ°ÇÌÇ©ÇÁÇÒ)
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

	// íeêßå‰
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->SetContext(_pEnemys);
			bullet->Update();
		}
	}

	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<SceneTitle>(_manager));
	}
}

void SceneMain::Draw()
{
	for (auto& enemy : _pEnemys)
	{
		if (enemy != nullptr)
		{
			enemy->Draw();
		}
	}
	_pPlayer->Draw();
	for (auto& bullet : _pBullets)
	{
		bullet->Draw();
	}

	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", _frameCount);
}