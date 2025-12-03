#include "EnemyManager.h"
#include "../Game.h"
#include "Dxlib.h"
#include <cassert>

#include "Camera.h"
#include "GimmickManager.h"
#include "EffectManager.h"

#include "../GameObjects/Enemies/WalkEnemy.h"
#include "../GameObjects/Enemies/FlyEnemy.h"
#include "../GameObjects/Enemies/JumpEnemy.h"
#include "../GameObjects/Enemies/DroneEnemy.h"

#include "../GameObjects/Enemies/WalkBoss.h"

#include "../Scenes/SceneMain.h"

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer,
	std::shared_ptr<Map> pMap,
	std::shared_ptr<Camera> pCamera,
	std::shared_ptr<GimmickManager> pGimmickManager,
	std::shared_ptr<EffectManager> pEffectManager,
	SceneMain& sceneMain,
	SceneManager& sceneManager):
	_pPlayer(pPlayer),
	_pMap(pMap),
	_pCamera(pCamera),
	_pGimmickManager(pGimmickManager),
	_pEffectManager(pEffectManager),
	_sceneMain(sceneMain),
	_sceneManager(sceneManager)
{
	_walkEnemyH = LoadGraph("data/Enemys/WalkEnemy.png");
	assert(_walkEnemyH != -1);
	_flyEnemyH = LoadGraph("data/Enemys/FlyEnemy.png");
	assert(_flyEnemyH != -1);
	_jumpEnemyH = LoadGraph("data/Enemys/JumpEnemy.png");
	assert(_jumpEnemyH != -1);
	_droneEnemyH = LoadGraph("data/Enemys/DroneEnemy.png");
	assert(_droneEnemyH != -1);
}

EnemyManager::~EnemyManager()
{
	DeleteGraph(_walkEnemyH);
	DeleteGraph(_flyEnemyH);
	DeleteGraph(_jumpEnemyH);
	DeleteGraph(_droneEnemyH);
}

void EnemyManager::Update()
{
	for (auto& enemy : _pEnemies)
	{
		if (enemy != nullptr)
		{
			float toCameraDisX = enemy->GetPos().x - _pCamera->GetPos().x;	// 敵とカメラの距離X
			float toCameraDisY = enemy->GetPos().y - _pCamera->GetPos().y;	// 敵とカメラの距離Y
			// カメラの画面内にいる敵だけ更新する
			if (abs(toCameraDisX) < GlobalConstants::kScreenWidth / 2 + 100 &&
				abs(toCameraDisY) < GlobalConstants::kScreenHeight / 2 + 100)
			{
				enemy->Update(*_pMap);
			}
			// 敵が死んだとき
			if (!enemy->GetIsAlive())
			{
				_sceneMain.AddScore(enemy->GetScore());	// スコア加算
				//_pEffectManager->Create(enemy->GetColliderPos(), EffectType::Explosion);	// エフェクト生成
			}
		}
	}

	// 死んだ敵をvectorから削除する
	_pEnemies.erase(
		std::remove_if(_pEnemies.begin(), _pEnemies.end(),
			[](const std::shared_ptr<Enemy>& enemy) {
				return !enemy->GetIsAlive();
			}),
		_pEnemies.end()
	);
}

void EnemyManager::Draw()
{
	for (auto& enemy : _pEnemies)
	{
		if (enemy != nullptr)
		{
			enemy->Draw(_pCamera->GetDrawOffset());
		}
	}
}

void EnemyManager::LoadEnemies(const std::vector<uint16_t>& objectData, Size size)
{ 
	for (int w = 0; w < size.w; w++)
	{
		for (int h = 0; h < size.h; h++)
		{
			auto index = w + h * size.w;
			auto data = static_cast<ObjectData>(objectData[index]);
			Vector2 pos = Vector2(static_cast<float>(w), static_cast<float>(h));
			switch (data)
			{
			case ObjectData::WalkEnemyIdle:
				_pEnemies.push_back(std::make_shared<WalkEnemy>(pos, _pPlayer,_pEffectManager, _sceneManager, _walkEnemyH, WalkEnemyState::Idle, false));
				break;
			case ObjectData::WalkEnemyMoveLeft:
				_pEnemies.push_back(std::make_shared<WalkEnemy>(pos, _pPlayer, _pEffectManager, _sceneManager, _walkEnemyH, WalkEnemyState::Move, true));
				break;
			case ObjectData::WalkEnemyMoveRight:
				_pEnemies.push_back(std::make_shared<WalkEnemy>(pos, _pPlayer, _pEffectManager, _sceneManager, _walkEnemyH, WalkEnemyState::Move, false));
				break;
			case ObjectData::FlyEnemyIdle:
				_pEnemies.push_back(std::make_shared<FlyEnemy>(pos, _pPlayer, _pEffectManager, _sceneManager, _flyEnemyH, FlyEnemyState::Idle));
				break;
			case ObjectData::FlyEnemyMove:
				_pEnemies.push_back(std::make_shared<FlyEnemy>(pos, _pPlayer, _pEffectManager, _sceneManager, _flyEnemyH, FlyEnemyState::Move));
				break;
			case ObjectData::JumpEnemy:
				_pEnemies.push_back(std::make_shared<JumpEnemy>(pos, _pPlayer, _pEffectManager, _sceneManager, _jumpEnemyH));
				break;
			case ObjectData::DroneEnemy:
				_pEnemies.push_back(std::make_shared<DroneEnemy>(pos, _pPlayer, _pEffectManager, _sceneManager, _droneEnemyH));
				break;
			case ObjectData::BossEnemy1:
				_pEnemies.push_back(std::make_shared<WalkBoss>(pos,_pPlayer, _pEffectManager, _pCamera, _pGimmickManager->AddBossLaser(),_sceneManager, _walkEnemyH));
				break;
			default:
				break;
			}
		}
	}
}