#include "SceneMain.h"

#include "DxLib.h"
#include "Application.h"
#include "input.h"
#include <cassert>
#include "Game.h"

#include "SceneManager.h"
#include "DebugScene.h"

#include "Player.h"
#include "Bullet.h"

#include "WalkEnemy.h"
#include "FlyEnemy.h"
#include "JumpEnemy.h"

#include "Item.h"

#include "Laser.h"

#include "Map.h"
#include "Camera.h"


namespace
{
	constexpr int BULLET_NUM = 15;
}

SceneMain::SceneMain(SceneManager& manager, Stages stage) :
	SceneBase(manager),
	_frameCount(0)
{
	/*画像の読み込み*/
	LoadAllGraphs();

	/*オブジェクトの生成*/
	LoadStage(stage);
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
	for (auto& enemy : _pEnemys)
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
	_pCamera->Update(_pPlayer->GetPos().x);

	// 敵制御
	for (auto& enemy : _pEnemys)
	{
		if (enemy != nullptr)
		{
			float toCameraDisX = enemy->GetPos().x - _pCamera->GetPos().x;
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
			bullet->SetContext(_pEnemys);
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
	// 背景色
	//DrawBox(0, 0, GlobalConstants::SCREEN_WIDTH, GlobalConstants::SCREEN_HEIGHT, 0xffffff, true);

	// マップの描画
	_pMap->Draw(_pCamera->GetDrawOffset());

	// ギミックの描画
	for (auto& gimmick : _pGimmicks)
	{
		gimmick->Draw(_pCamera->GetDrawOffset());
	}
	
	// 敵の描画
	for (auto& enemy : _pEnemys)
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

	for (auto& item : _pItems)
	{
		item->Draw(_pCamera->GetDrawOffset());
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
	_pPlayer = std::make_shared<Player>(_playerH, _playerWhiteH, _chargeParticleH);
	// 弾
	_pBullets.resize(BULLET_NUM);
	for (auto& bullet : _pBullets)
	{
		bullet = std::make_shared<Bullet>(_playerShotH, _chargeShotH);
	}
	// マップ
	_pMap = std::make_shared<Map>(_mapChipH);

	switch (stage)
	{
	case Stages::Temp:
		printfDx("Stages::Tempがロードされました\n");

		_pPlayer->SetPosFromMapChip({ 3,19 });

		_pMap->LoadMapData("data/Map/TempMap.csv");

		_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(20,18), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
		_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(20, 15), _pPlayer, _flyEnemyH, FlyEnemyState::Idle));
		_pEnemys.push_back(std::make_shared<JumpEnemy>(Vector2(24, 18), _pPlayer, _jumpEnemyH));

		_pItems.push_back(std::make_shared<Item>(Vector2(17, 17), ItemType::Coin, _pPlayer, _coinH));
		_pItems.push_back(std::make_shared<Item>(Vector2(19, 17), ItemType::BigCoin, _pPlayer, _bigCoinH));
		_pItems.push_back(std::make_shared<Item>(Vector2(21, 17), ItemType::HealthItem, _pPlayer, _healthItemH));

		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(10, 12), _pPlayer, _laserH, 4));

		break;
	case Stages::Tutorial:
		printfDx("Stages::Tutorialがロードされました\n");

		// プレイヤーの位置を設定
		_pPlayer->SetPos({ 3 * 48,19 * 48 });

		// マップのデータをロード
		_pMap->LoadMapData("data/Map/TutorialMap.csv");

		// 敵を生成
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(24 * 48, 19 * 48), WalkEnemyState::Idle, false, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(51 * 48, 16 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(60 * 48, 15 * 48), _flyEnemyH, FlyEnemyState::Idle, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(57 * 48, 19 * 48), WalkEnemyState::Move, false, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(77 * 48, 19 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(94 * 48, 13 * 48), _flyEnemyH, FlyEnemyState::Idle,_pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(97 * 48, 13 * 48), _flyEnemyH, FlyEnemyState::Idle, _pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(100 * 48, 13 * 48), _flyEnemyH, FlyEnemyState::Idle, _pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(103 * 48, 13 * 48), _flyEnemyH, FlyEnemyState::Idle, _pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(106 * 48, 13 * 48), _flyEnemyH, FlyEnemyState::Idle, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(115 * 48, 7 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(142 * 48, 9 * 48), WalkEnemyState::Idle, false, _walkEnemyH, _pPlayer));

		//// レーザー
		//_pGimmicks.push_back(std::make_shared<Laser>(Vector2(139 * 48, 12 * 48), _laserH, 7, _pPlayer));

		break;
	case Stages::Stage1 :
		printfDx("Stages::Stage1がロードされました\n");

		_pPlayer->SetPos({ 3 * 48,19 * 48 });

		_pMap->LoadMapData("data/Map/Stage1Map.csv");

		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(23 * 48, 19 * 48), WalkEnemyState::Idle, false, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(28 * 48, 15 * 48), WalkEnemyState::Idle, false, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(31 * 48, 19 * 48), WalkEnemyState::Move, false, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(46 * 48, 19 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(42 * 48, 14 * 48), _flyEnemyH, FlyEnemyState::Move, _pPlayer));
		//_pEnemys.push_back(std::make_shared<FlyEnemy>(Vector2(56 * 48, 9 * 48), _flyEnemyH, FlyEnemyState::Idle, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(69 * 48, 13 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(61 * 48, 19 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(64 * 48, 19 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(67 * 48, 19 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<WalkEnemy>(Vector2(88 * 48, 16 * 48), WalkEnemyState::Move, true, _walkEnemyH, _pPlayer));
		//_pEnemys.push_back(std::make_shared<JumpEnemy>(Vector2(96 * 48, 19 * 48), _pPlayer, _jumpEnemyH));

		break;
	case Stages::Stage2:
		printfDx("Stages::Stage2がロードされました\n");

		_pPlayer->SetPos({ 3 * 48,19 * 48 });

		_pMap->LoadMapData("data/Map/TempMap.csv");

		break;
	case Stages::Boss:
		printfDx("Stages::Bossがロードされました\n");

		_pPlayer->SetPos({ 3 * 48,19 * 48 });

		_pMap->LoadMapData("data/Map/TempMap.csv");

		break;
	}

	// カメラ(マップの幅を取得する必要があるためマップを含む諸々生成してから生成)
	_pCamera = std::make_shared<Camera>(_pMap->GetStageWidth());
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
}
