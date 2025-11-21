#include "SceneMain.h"

#include "DxLib.h"
#include "Application.h"
#include "input.h"
#include <cassert>
#include "Game.h"

#include "Collider.h"

#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneClear.h"
#include "SceneGameOver.h"

#include "Player.h"
#include "Bullet.h"

#include "WalkEnemy.h"
#include "FlyEnemy.h"
#include "JumpEnemy.h"
#include "Boss.h"

#include "Item.h"

#include "ClearFlag.h"
#include "Laser.h"

#include "Map.h"
#include "Camera.h"

#include "HPUI.h"
#include "BossHPUI.h"

#include "Bg.h"

namespace
{
	constexpr int BULLET_NUM = 15;
}

SceneMain::SceneMain(SceneManager& manager, Stages stage) :
	SceneBase(manager),
	_frameCount(0),
	_nowStage(stage)
{
	/*画像の読み込み*/
	LoadAllGraphs();

	/*オブジェクトの生成*/
	LoadStage(stage);

#ifdef _DEBUG
	if (_pClearFlag == nullptr)
	{
		printfDx("ゴール旗がnullptrです");
	}
	if (_pBg == nullptr)
	{
		printfDx("背景がnullptrです");
	}
#endif
}

SceneMain::~SceneMain()
{
	DeleteAllGraphs();
}

void SceneMain::Init()
{
	_pPlayer->Init();
	for (auto& bullet : _pBullets)
	{
		bullet->Init();
	}
	for (auto& enemy : _pEnemies)
	{
		enemy->Init();
	}
}

void SceneMain::Update(Input& input)
{
	_frameCount++;

	// プレイヤー制御
	_pPlayer->SetContext(input,_pBullets);
	_pPlayer->Update(*_pMap);
	// カメラ制御
	_pCamera->Update(_pPlayer->GetPos());

	// プレイヤーが死んだときの処理
	if (!_pPlayer->GetIsAlive())
	{
		_manager.ChangeScene(std::make_shared<SceneGameOver>(_manager, _nowStage));
		return;
	}

	// 敵制御
	for (auto& enemy : _pEnemies)
	{
		if (enemy != nullptr)
		{
			float toCameraDisX = enemy->GetPos().x - _pCamera->GetPos().x;	// 敵とカメラの距離
			// カメラの画面内にいる敵だけ更新する
			if (abs(toCameraDisX) < GlobalConstants::SCREEN_WIDTH / 2 + 100)
			{
				enemy->Update(*_pMap);
			}
			// 体力が0以下になったら消す
			if (enemy->GetHp() <= 0)
			{
				enemy->Delete();
				// 死んだ敵をvectorから削除する(あんま意味わからん)
				_pEnemies.erase(
					std::remove_if(_pEnemies.begin(), _pEnemies.end(),
						[](const std::shared_ptr<Enemy>& enemy) {
							return !enemy->GetIsAlive();
						}),
					_pEnemies.end()
				);
			}
		}
	}

	// ギミック
	for (auto& gimmick : _pGimmicks)
	{
		gimmick->Update(*_pMap);
	}

	// 弾制御
	for (auto& bullet : _pBullets)
	{
		if (bullet->GetAlive())
		{
			bullet->SetContext(_pEnemies);
			bullet->Update(*_pMap,_pCamera->GetPos());
		}
	}

	// アイテム制御
	for (auto& item : _pItems)
	{
		if (item != nullptr)
		{
			item->Update(*_pMap);
			if (!item->GetIsAlive())
			{	// 取られたアイテムをvectorから削除する(あんま意味わからん)
				_pItems.erase(
					std::remove_if(_pItems.begin(), _pItems.end(),
						[](const std::shared_ptr<Item>& item) {
							return !item->GetIsAlive();
						}),
					_pItems.end()
				);
			}
		}
	}

	_pHPUI->Update(_pPlayer->GetHp());
	if (_pBossHPUI != nullptr)
	{
		if (_pEnemies.empty())
		{
			_pBossHPUI->Update(0);
		}
		else
		{
			_pBossHPUI->Update(_pEnemies.back()->GetHp());
		}
	}

	// ゴール判定
#ifdef _DEBUG
	if (_pClearFlag != nullptr)
	{
		if (_pPlayer->GetCollider()->CheckCollision(_pClearFlag->GetCollider()))
		{	// プレイヤーとゴール旗が接触したらステージクリアへ
			StageClear();
		}
	}
#else
	if (_pClearFlag != nullptr)
	{
		if (_pPlayer->GetCollider()->CheckCollision(_pClearFlag->GetCollider()))
		{	// プレイヤーとゴール旗が接触したらステージクリアへ
			StageClear();
		}
	}
	else
	{
		assert(false, "ゴール旗が存在しません");
	}
#endif

	// 【デバッグ用】デバッグシーンに切り替え
#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
#endif
}

void SceneMain::Draw()
{

	// 背景の描画
	_pBg->Draw(_pCamera->GetDrawOffset());

	// マップの描画
	_pMap->Draw(_pCamera->GetDrawOffset());

	// ギミックの描画
	for (auto& gimmick : _pGimmicks)
	{
		gimmick->Draw(_pCamera->GetDrawOffset());
	}
	
	// 敵の描画
	for (auto& enemy : _pEnemies)
	{
		if (enemy != nullptr)
		{
			enemy->Draw(_pCamera->GetDrawOffset());
		}
	}
	// プレイヤーの描画
	_pPlayer->Draw(_pCamera->GetDrawOffset());
	// 弾の描画
	for (auto& bullet : _pBullets)
	{
		bullet->Draw(_pCamera->GetDrawOffset());
	}

	// アイテムの描画
	for (auto& item : _pItems)
	{
		item->Draw(_pCamera->GetDrawOffset());
	}

	// ゴール旗の描画
	if (_pClearFlag != nullptr)
	{
		_pClearFlag->Draw(_pCamera->GetDrawOffset());
	}

	_pHPUI->Draw(_pPlayer->GetPos() - _pCamera->GetDrawOffset(),_pEnemies);
	if (_pBossHPUI != nullptr)
	{
		_pBossHPUI->Draw(_pPlayer->GetPos() - _pCamera->GetDrawOffset(), _pEnemies);
	}

#ifdef _DEBUG
	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", _frameCount);
#endif // _DEBUG
}

void SceneMain::LoadStage(Stages stage)
{
	// 先に必ず存在するオブジェクトの生成をする
	// プレイヤー
	_pPlayer = std::make_shared<Player>(_playerH, _playerWhiteH, _chargeParticleH,_playerShotH,_chargeShotH);
	// 弾
	_pBullets.resize(BULLET_NUM);
	for (auto& bullet : _pBullets)
	{
		bullet = std::make_shared<Bullet>(_playerShotH, _chargeShotH);
	}
	// マップ
	_pMap = std::make_shared<Map>(_mapChipH);

	// HPUI
	_pHPUI = std::make_shared<HPUI>(_hpUIH,_pPlayer->GetMaxHp());

	// カメラ(最後にいろいろ設定する必要がある)
	_pCamera = std::make_shared<Camera>();

	// 背景
	_pBg = std::make_shared<Bg>(_bgH,_subBgH);

	switch (stage)
	{
	case Stages::Temp:
		//printfDx("Stages::Tempがロードされました\n");

		_pPlayer->SetPosFromChipPos({ 3,19 });

		_pMap->LoadMapData("data/Map/TempMap.csv");

		_pClearFlag = std::make_shared<ClearFlag>(Vector2(68, 18), _pPlayer, _clearFlagH);

		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(20,18), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(18, 18), _pPlayer, _walkEnemyH, WalkEnemyState::Move, false));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(20, 15), _pPlayer, _flyEnemyH, FlyEnemyState::Idle));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(23, 15), _pPlayer, _flyEnemyH, FlyEnemyState::Move));
		//_pEnemys.push_back(std::make_shared<JumpEnemy>(Vector2(24, 18), _pPlayer, _jumpEnemyH));
		//_pEnemys.push_back(std::make_shared<JumpEnemy>(Vector2(58, 18), _pPlayer, _jumpEnemyH));

		_pItems.push_back(std::make_shared<Item>(Vector2(17, 17), ItemType::Coin, _pPlayer, _coinH));
		_pItems.push_back(std::make_shared<Item>(Vector2(19, 17), ItemType::BigCoin, _pPlayer, _bigCoinH));
		_pItems.push_back(std::make_shared<Item>(Vector2(21, 17), ItemType::HealthItem, _pPlayer, _healthItemH));

		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(11, 12), _pPlayer, _laserH, 4,false));
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(12, 12), _pPlayer, _laserH, 7));

		break;
	case Stages::Tutorial: // ------------------------------------------------------------------------------------------チュートリアル
		// プレイヤーの位置を設定
		_pPlayer->SetPosFromChipPos({ 3 ,36});

		// マップのデータをロード
		_pMap->LoadMapData("data/Map/TutorialMap.csv");

		// ゴール旗を生成
		_pClearFlag = std::make_shared<ClearFlag>(Vector2(166,22), _pPlayer, _clearFlagH);

		// 敵を生成
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(39, 35), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(61, 31), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(64, 31), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(67, 31), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(70, 31), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(73, 31), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));

		// ギミックの生成
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(116, 32), _pPlayer, _laserH, 4));

#ifdef _DEBUG
		//printfDx("Stages::Tutorialがロードされました\n");
#endif
		break;
	case Stages::Stage1: // ------------------------------------------------------------------------------------------ステージ1
		_pPlayer->SetPosFromChipPos({ 3 ,36  });

		_pMap->LoadMapData("data/Map/Stage1Map.csv");

		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(23, 35), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(28, 31), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(31, 35), _pPlayer, _walkEnemyH, WalkEnemyState::Move, false));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(46, 35), _pPlayer, _walkEnemyH, WalkEnemyState::Move, true));
		_pEnemies.push_back(std::make_shared<FlyEnemy>(Vector2(42, 30), _pPlayer, _flyEnemyH, FlyEnemyState::Move));
		_pEnemies.push_back(std::make_shared<FlyEnemy>(Vector2(56, 25), _pPlayer, _flyEnemyH, FlyEnemyState::Idle));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(69, 29), _pPlayer, _walkEnemyH, WalkEnemyState::Move, true));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(61, 35), _pPlayer, _walkEnemyH, WalkEnemyState::Move, true));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(64, 35), _pPlayer, _walkEnemyH, WalkEnemyState::Move, true));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(67, 35), _pPlayer, _walkEnemyH, WalkEnemyState::Move, true));
		_pEnemies.push_back(std::make_shared<WalkEnemy>(Vector2(88, 32), _pPlayer, _walkEnemyH, WalkEnemyState::Move, true));
		_pEnemies.push_back(std::make_shared<JumpEnemy>(Vector2(96, 35), _pPlayer, _jumpEnemyH));

		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(63, 32), _pPlayer, _laserH, 4));
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(124, 31), _pPlayer, _laserH, 5));
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(135, 31), _pPlayer, _laserH, 5));

#ifdef _DEBUG
		//printfDx("Stages::Stage1がロードされました\n");
#endif
		break;
	case Stages::Stage2: // ------------------------------------------------------------------------------------------ステージ2
		_pPlayer->SetPosFromChipPos({ 3,19 });

		_pMap->LoadMapData("data/Map/TempMap.csv");

#ifdef _DEBUG
		//printfDx("Stages::Stage2がロードされました\n");
#endif
		break;
	case Stages::Boss: // ------------------------------------------------------------------------------------------ボスステージ
		_pPlayer->SetPosFromChipPos({ 3,19 });

		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(-1, -1), _pPlayer, _laserH, 14, false));
		_pEnemies.push_back(std::make_shared<Boss>(_pPlayer, _pCamera, _pGimmicks[0], _walkEnemyH));

		_pBossHPUI = std::make_shared<BossHPUI>(_bossHpUIH,_pEnemies.back()->GetHp());

		_pMap->LoadMapData("data/Map/BossStage.csv");

#ifdef _DEBUG
		//printfDx("Stages::Bossがロードされました\n");
#endif
		break;
	}

	// カメラの初期設定(ステージサイズなどをセットする必要があるため、最後に行う)
	_pCamera->SetStageSize(_pMap->GetStageSize());
	_pCamera->SetPos(Vector2(GlobalConstants::SCREEN_WIDTH / 2,_pMap->GetStageSize().y - GlobalConstants::SCREEN_HEIGHT / 2));
}

void SceneMain::StageClear()
{
	_manager.ChangeScene(std::make_shared<SceneClear>(_manager));
	return;
}

void SceneMain::LoadAllGraphs()
{
	_playerH = LoadGraph("data/Player/Player.png");
	assert(_playerH != -1);
	_playerWhiteH = LoadGraph("data/Player/PlayerWhite.png");
	assert(_playerWhiteH != -1);
	_playerShotH = LoadGraph("data/Player/Shot.png");
	assert(_playerShotH != -1);
	_chargeShotH = LoadGraph("data/Player/ChargeShot.png");
	assert(_chargeShotH != -1);
	_chargeParticleH = LoadGraph("data/Player/ChargeParticle.png");
	assert(_chargeParticleH != -1);
	_walkEnemyH = LoadGraph("data/Enemys/WalkEnemy.png");
	assert(_walkEnemyH != -1);
	_flyEnemyH = LoadGraph("data/Enemys/FlyEnemy.png");
	assert(_flyEnemyH != -1);
	_jumpEnemyH = LoadGraph("data/Enemys/JumpEnemy.png");
	assert(_jumpEnemyH != -1);
	_mapChipH = LoadGraph("data/Map/MapChip.png");
	assert(_mapChipH != -1);
	_laserH = LoadGraph("data/Gimmicks/Laser.png");
	assert(_laserH != -1);
	_coinH = LoadGraph("data/Items/Coin.png");
	assert(_coinH != -1);
	_bigCoinH = LoadGraph("data/Items/BigCoin.png");
	assert(_bigCoinH != -1);
	_healthItemH = LoadGraph("data/Items/HealthItem.png");
	assert(_healthItemH != -1);
	_clearFlagH = LoadGraph("data/ClearFlag.png");
	assert(_clearFlagH != -1);
	_hpUIH = LoadGraph("data/UI/HpBar.png");
	assert(_hpUIH != -1);
	_bossHpUIH = LoadGraph("data/UI/BossHpBar.png");
	assert(_bossHpUIH != -1);
	_bgH = LoadGraph("data/Map/Bg.png");
	assert(_bgH != -1);
	_subBgH = LoadGraph("data/Map/subBg.png");
	assert(_subBgH != -1);
}

void SceneMain::DeleteAllGraphs()
{
	DeleteGraph(_playerH);
	DeleteGraph(_playerShotH);
	DeleteGraph(_chargeShotH);
	DeleteGraph(_chargeParticleH);
	DeleteGraph(_walkEnemyH);
	DeleteGraph(_flyEnemyH);
	DeleteGraph(_mapChipH);
	DeleteGraph(_laserH);
	DeleteGraph(_coinH);
	DeleteGraph(_bigCoinH);
	DeleteGraph(_healthItemH);
	DeleteGraph(_clearFlagH);
	DeleteGraph(_hpUIH);
	DeleteGraph(_bgH);
}
