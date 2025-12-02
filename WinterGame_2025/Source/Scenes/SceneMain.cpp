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
#include "ScenePause.h"

#include "../GameObjects/Player.h"
//#include "../GameObjects/Bullet.h"

#include "../GameObjects/Enemies/Enemy.h"
//#include "../GameObjects/Enemies/WalkEnemy.h"
//#include "../GameObjects/Enemies/FlyEnemy.h"
//#include "../GameObjects/Enemies/JumpEnemy.h"
//#include "../GameObjects/Enemies/WalkBoss.h"

//#include "../GameObjects/Item.h"

#include "../GameObjects/ClearFlag.h"
//#include "../GameObjects/Laser.h"

#include "../UI/HPUI.h"
#include "../UI/BossHPUI.h"

#include "../Systems/Map.h"
#include "../Systems/Camera.h"
#include "../Systems/Bg.h"

#include "../Systems/EnemyManager.h"
#include "../Systems/GimmickManager.h"
#include "../Systems/ItemManager.h"
#include "../Systems/BulletManager.h"
#include "../Systems/EffectManager.h"

namespace
{
	//constexpr int kBulletNum = 0;

	// 画像リスト
	enum class Graphs
	{
		Player,
		PlayerWhite,
		PlayerShot,
		ChargeShot,
		ChargeParticle,
		MapChip,
		ClearFlag,
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
		"data/Map/MapChip.png",
		"data/ClearFlag.png",
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
}

void SceneMain::Update(Input& input)
{
	_frameCount++;

	if (input.IsTriggered("start"))
	{	// ポーズシーンへ切り替え
		_manager.PushScene(std::make_shared<ScenePause>(_manager));
		return;
	}

	// プレイヤー制御
	_pPlayer->SetContext(input);
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
	_pGimmickManager->Update();

	// 弾制御
	_pBulletManager->Update(*_pMap, _pCamera->GetPos(), _pEnemyManager->GetEnemies());

	// アイテム制御
	_pItemManager->Update();

	_pEffectManager->Update();

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
	_pGimmickManager->Draw(_pCamera->GetDrawOffset());
	
	// 敵の描画
	_pEnemyManager->Draw();

	// プレイヤーの描画
	_pPlayer->Draw(_pCamera->GetDrawOffset());

	// 弾の描画
	_pBulletManager->Draw(_pCamera->GetDrawOffset());

	// アイテムの描画
	_pItemManager->Draw(_pCamera->GetDrawOffset());

	_pEffectManager->Draw(_pCamera->GetDrawOffset());

	// ゴール旗の描画
	if (_pClearFlag != nullptr)
	{
		_pClearFlag->Draw(_pCamera->GetDrawOffset());
	}

	// UIの描画
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
	/*ステージデータのロード*/
	_pStage = std::make_shared<Stage>();
	switch (stage)
	{
	case Stages::Temp:
		_pStage->LoadData("data/Stages/TempStage.fmf");
		break;
	case Stages::Tutorial:
		_pStage->LoadData("data/Stages/TutorialStage.fmf");
		break;
	case Stages::Stage1:
		_pStage->LoadData("data/Stages/template.fmf");
		break;
	case Stages::Stage2:
		_pStage->LoadData("data/Stages/template.fmf");
		break;
	case Stages::Boss:
		_pStage->LoadData("data/Stages/BossStage.fmf");
		break;
	default:
		break;
	}

	// オブジェクトの生成、初期化
	
	_pEffectManager = std::make_shared<EffectManager>();

	// 弾
	_pBulletManager = std::make_shared<BulletManager>();

	// プレイヤー
	_pPlayer = std::make_shared<Player>(_graphHandles[static_cast<int>(Graphs::Player)], _graphHandles[static_cast<int>(Graphs::PlayerWhite)], _graphHandles[static_cast<int>(Graphs::ChargeParticle)], _graphHandles[static_cast<int>(Graphs::PlayerShot)], _graphHandles[static_cast<int>(Graphs::ChargeShot)], _pBulletManager);
	_pPlayer->InitPosFromStage(_pStage->GetObjectData(), _pStage->GetMapSize());		// プレイヤーの位置を設定

	// マップ
	_pMap = std::make_shared<Map>(_graphHandles[static_cast<int>(Graphs::MapChip)]);
	_pMap->SetMapData(_pStage->GetMapData(), _pStage->GetMapSize());		// マップデータを設定

	// HPUI
	_pHPUI = std::make_shared<HPUI>(_graphHandles[static_cast<int>(Graphs::HpUI)], _pPlayer->GetMaxHp());

	// カメラ
	_pCamera = std::make_shared<Camera>();
	_pCamera->SetStageSize(_pMap->GetStageSize());
	_pCamera->SetPos(Vector2(GlobalConstants::kScreenWidth / 2, _pMap->GetStageSize().y - GlobalConstants::kScreenHeight / 2));

	// 背景
	_pBg = std::make_shared<Bg>(_graphHandles[static_cast<int>(Graphs::Bg)], _graphHandles[static_cast<int>(Graphs::SubBg)]);

	// ギミック
	_pGimmickManager = std::make_shared<GimmickManager>(_pPlayer);
	_pGimmickManager->LoadGimmicks(_pStage->GetObjectData(), _pStage->GetMapSize());// ギミックの生成

	// アイテム
	_pItemManager = std::make_shared<ItemManager>(_pPlayer);
	_pItemManager->LoadItems(_pStage->GetObjectData(), _pStage->GetMapSize());	// アイテムの生成

	// 敵
	_pEnemyManager = std::make_shared<EnemyManager>(_pPlayer, _pMap, _pCamera,_pGimmickManager);
	_pEnemyManager->LoadEnemies(_pStage->GetObjectData(), _pStage->GetMapSize());// 敵の生成
	if (stage == Stages::Boss)	// ボスステージならボスHPUIの生成
	{
		_pBossHPUI = std::make_shared<BossHPUI>(_graphHandles[static_cast<int>(Graphs::BossHpUI)], _pEnemyManager->GetEnemies().back()->GetHp());
	}

	// ゴール旗を生成
	_pClearFlag = std::make_shared<ClearFlag>(Vector2(-10,-10), _pPlayer, _graphHandles[static_cast<int>(Graphs::ClearFlag)]);
	_pClearFlag->InitPosFromStage(_pStage->GetObjectData(), _pStage->GetMapSize());
}

void SceneMain::StageClear()
{
	_manager.ChangeSceneWithFade(std::make_shared<SceneClear>(_manager), FadeState::NormalFadeIn, FadeState::CircleFadeOut);
	return;
}