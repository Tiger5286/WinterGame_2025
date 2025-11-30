#include "SceneMain.h"

#include "DxLib.h"
#include "../Application.h"
#include "../Systems/input.h"
#include <cassert>
#include "../Game.h"

#include "../Utility/Collider.h"
#include "../Systems/Stage.h"

#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneClear.h"
#include "SceneGameOver.h"

#include "../GameObjects/Player.h"
#include "../GameObjects/Bullet.h"

#include "../GameObjects/Enemies/WalkEnemy.h"
#include "../GameObjects/Enemies/FlyEnemy.h"
#include "../GameObjects/Enemies/JumpEnemy.h"
#include "../GameObjects/Enemies/Boss.h"

#include "../GameObjects/Item.h"

#include "../GameObjects/ClearFlag.h"
#include "../GameObjects/Laser.h"

#include "../UI/HPUI.h"
#include "../UI/BossHPUI.h"

#include "../Systems/Map.h"
#include "../Systems/Camera.h"
#include "../Systems/Bg.h"

#include "../Systems/EnemyManager.h"

namespace
{
	constexpr int kBulletNum = 15;

	// 画像リスト
	enum class Graphs
	{
		Player,
		PlayerWhite,
		PlayerShot,
		ChargeShot,
		ChargeParticle,
		WalkEnemy,
		FlyEnemy,
		JumpEnemy,
		MapChip,
		ClearFlag,
		Laser,
		Coin,
		BigCoin,
		HealthItem,
		HpUI,
		BossHpUI,
		Bg,
		SubBg,

		Num
	};

	// ロードする画像のファイル名
	const std::string kGraphFileNames[] =
	{
		"data/Player/Player.png",
		"data/Player/PlayerWhite.png",
		"data/Player/Shot.png",
		"data/Player/ChargeShot.png",
		"data/Player/ChargeParticle.png",
		"data/Enemys/WalkEnemy.png",
		"data/Enemys/FlyEnemy.png",
		"data/Enemys/JumpEnemy.png",
		"data/Map/MapChip.png",
		"data/ClearFlag.png",
		"data/Gimmicks/Laser.png",
		"data/Items/Coin.png",
		"data/Items/BigCoin.png",
		"data/Items/HealthItem.png",
		"data/UI/HpBar.png",
		"data/UI/BossHpBar.png",
		"data/Map/Bg.png",
		"data/Map/subBg.png",
	};

	// Graphsで定義した画像数とkGraphFileNamesで定義したファイル名の要素数が一致していなかったらエラー
	constexpr int size = sizeof(kGraphFileNames) / sizeof(kGraphFileNames[0]);	// 配列全体のサイズ / 配列の要素一つのサイズ = 配列の要素数
	static_assert(static_cast<int>(Graphs::Num) == size);
}

SceneMain::SceneMain(SceneManager& manager, Stages stage) :
	SceneBase(manager),
	_frameCount(0),
	_nowStage(stage)
{
	/*画像の読み込み*/
	_graphHandles.resize(static_cast<int>(Graphs::Num));
	for (int i = 0; i < static_cast<int>(Graphs::Num); i++)
	{
		_graphHandles[i] = LoadGraph(kGraphFileNames[i].c_str());
		assert(_graphHandles.back() != -1);
	}

	/*ステージのロードと生成*/
	LoadStage(stage);

	// フェードサークルの位置更新(プレイヤーの位置から始まる)
	_manager.SetFadeCirclePos(_pPlayer->GetPos() - _pCamera->GetDrawOffset());

#ifdef _DEBUG
	if (_pClearFlag == nullptr)
	{
		printfDx("ゴール旗がnullptrです\n");
	}
	if (_pBg == nullptr)
	{
		printfDx("背景がnullptrです\n");
	}
#endif
}

SceneMain::~SceneMain()
{
	// 画像をメモリから解放
	for (auto& handle : _graphHandles)
	{
		DeleteGraph(handle);
	}
}

void SceneMain::Init()
{
	_pPlayer->Init();
	for (auto& bullet : _pBullets)
	{
		bullet->Init();
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
		_manager.ChangeSceneWithFade(std::make_shared<SceneGameOver>(_manager, _nowStage),FadeState::NormalFadeIn,FadeState::CircleFadeOut);
		return;
	}

	// フェードサークルの位置更新
	_manager.SetFadeCirclePos(_pPlayer->GetPos() - _pCamera->GetDrawOffset());

	// 敵制御
	_pEnemyManager->Update();

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
			bullet->SetContext(_pEnemyManager->GetEnemies());
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

	// HPUI更新
	_pHPUI->Update(_pPlayer->GetHp());
	if (_pBossHPUI != nullptr)
	{
		if (_pEnemyManager->GetEnemies().empty())
		{
			_pBossHPUI->Update(0);	// 敵がいなくなったらHP0にする
		}
		else
		{
			_pBossHPUI->Update(_pEnemyManager->GetEnemies().back()->GetHp());	// 最後にいる敵をボスとみなす
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
	_pEnemyManager->Draw();

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

	_pHPUI->Draw(_pPlayer->GetPos() - _pCamera->GetDrawOffset(), _pEnemyManager->GetEnemies());
	if (_pBossHPUI != nullptr)
	{
		_pBossHPUI->Draw(_pPlayer->GetPos() - _pCamera->GetDrawOffset(), _pEnemyManager->GetEnemies());
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
	_pPlayer = std::make_shared<Player>(_graphHandles[static_cast<int>(Graphs::Player)], _graphHandles[static_cast<int>(Graphs::PlayerWhite)], _graphHandles[static_cast<int>(Graphs::ChargeParticle)], _graphHandles[static_cast<int>(Graphs::PlayerShot)], _graphHandles[static_cast<int>(Graphs::ChargeShot)]);
	// 弾
	_pBullets.resize(kBulletNum);
	for (auto& bullet : _pBullets)
	{
		bullet = std::make_shared<Bullet>(_graphHandles[static_cast<int>(Graphs::PlayerShot)], _graphHandles[static_cast<int>(Graphs::ChargeShot)]);
	}
	// マップ
	//_pMap = std::make_shared<Map>(_mapChipH);
	_pMap = std::make_shared<Map>(_graphHandles[static_cast<int>(Graphs::MapChip)]);

	// HPUI
	_pHPUI = std::make_shared<HPUI>(_graphHandles[static_cast<int>(Graphs::HpUI)], _pPlayer->GetMaxHp());

	// カメラ(最後にいろいろ設定する必要がある)
	_pCamera = std::make_shared<Camera>();

	// 背景
	_pBg = std::make_shared<Bg>(_graphHandles[static_cast<int>(Graphs::Bg)], _graphHandles[static_cast<int>(Graphs::SubBg)]);

	_pEnemyManager = std::make_shared<EnemyManager>(_pPlayer, _pMap, _pCamera);

	/*ステージデータのロード*/
	_pStage = std::make_shared<Stage>();
	switch (stage)
	{
	case Stages::Tutorial:
		_pStage->LoadData("data/Stages/TutorialStage.fmf");
		_pMap->SetMapData(_pStage->GetMapData(), _pStage->GetMapSize());
		break;
	default:
		break;
	}

	switch (stage)
	{
	case Stages::Temp:
		//printfDx("Stages::Tempがロードされました\n");

		_pPlayer->SetPosFromChipPos({ 3,19 });

		_pMap->LoadMapData("data/Map/TempMap.csv");

		_pClearFlag = std::make_shared<ClearFlag>(Vector2(68, 18), _pPlayer, _graphHandles[static_cast<int>(Graphs::ClearFlag)]);

		_pItems.push_back(std::make_shared<Item>(Vector2(17, 17), ItemType::Coin, _pPlayer, _graphHandles[static_cast<int>(Graphs::Coin)]));
		_pItems.push_back(std::make_shared<Item>(Vector2(19, 17), ItemType::BigCoin, _pPlayer, _graphHandles[static_cast<int>(Graphs::BigCoin)]));
		_pItems.push_back(std::make_shared<Item>(Vector2(21, 17), ItemType::HealthItem, _pPlayer, _graphHandles[static_cast<int>(Graphs::HealthItem)]));

		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(11, 12), _pPlayer, _graphHandles[static_cast<int>(Graphs::Laser)], 4, false));
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(12, 12), _pPlayer, _graphHandles[static_cast<int>(Graphs::Laser)], 7));

		break;
	case Stages::Tutorial: // ------------------------------------------------------------------------------------------チュートリアル
		// プレイヤーの位置を設定
		//_pPlayer->SetPosFromChipPos({ 3 ,36});
		_pPlayer->Spawn(_pStage->GetObjectData(), _pStage->GetMapSize());
		
		// 敵の生成
		_pEnemyManager->Spawn(_pStage->GetObjectData(), _pStage->GetMapSize());

		// ゴール旗を生成
		_pClearFlag = std::make_shared<ClearFlag>(Vector2(166,22), _pPlayer, _graphHandles[static_cast<int>(Graphs::ClearFlag)]);

		// ギミックの生成
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(116, 32), _pPlayer, _graphHandles[static_cast<int>(Graphs::Laser)], 4));

#ifdef _DEBUG
		//printfDx("Stages::Tutorialがロードされました\n");
#endif
		break;
	case Stages::Stage1: // ------------------------------------------------------------------------------------------ステージ1
		_pPlayer->SetPosFromChipPos({ 3 ,36  });

		_pMap->LoadMapData("data/Map/Stage1Map.csv");

		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(63, 32), _pPlayer, _graphHandles[static_cast<int>(Graphs::Laser)], 4));
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(124, 31), _pPlayer, _graphHandles[static_cast<int>(Graphs::Laser)], 5));
		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(135, 31), _pPlayer, _graphHandles[static_cast<int>(Graphs::Laser)], 5));

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

		_pGimmicks.push_back(std::make_shared<Laser>(Vector2(-1, -1), _pPlayer, _graphHandles[static_cast<int>(Graphs::Laser)], 14, false));
		//_pEnemies.push_back(std::make_shared<Boss>(_pPlayer, _pCamera, _pGimmicks[0], _graphHandles[static_cast<int>(Graphs::WalkEnemy)]));

		_pBossHPUI = std::make_shared<BossHPUI>(_graphHandles[static_cast<int>(Graphs::BossHpUI)], _pEnemyManager->GetEnemies().back()->GetHp());

		_pMap->LoadMapData("data/Map/BossStage.csv");

#ifdef _DEBUG
		//printfDx("Stages::Bossがロードされました\n");
#endif
		break;
	}

	// カメラの初期設定(ステージサイズなどをセットする必要があるため、最後に行う)
	_pCamera->SetStageSize(_pMap->GetStageSize());
	_pCamera->SetPos(Vector2(GlobalConstants::kScreenWidth / 2,_pMap->GetStageSize().y - GlobalConstants::kScreenHeight / 2));
}

void SceneMain::StageClear()
{
	_manager.ChangeSceneWithFade(std::make_shared<SceneClear>(_manager), FadeState::NormalFadeIn, FadeState::CircleFadeOut);
	return;
}

void SceneMain::LoadAllGraphs()
{
	// 画像をロード
#if false
	_graphHandles.push_back(LoadGraph("data/Player/Player.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Player/PlayerWhite.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Player/Shot.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Player/ChargeShot.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Player/ChargeParticle.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Enemys/WalkEnemy.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Enemys/FlyEnemy.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Enemys/JumpEnemy.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Map/MapChip.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/ClearFlag.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Gimmicks/Laser.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Items/Coin.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Items/BigCoin.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Items/HealthItem.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/UI/HpBar.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/UI/BossHpBar.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Map/Bg.png"));
	assert(_graphHandles.back() != -1);
	_graphHandles.push_back(LoadGraph("data/Map/subBg.png"));
	assert(_graphHandles.back() != -1);
#else

#endif
}