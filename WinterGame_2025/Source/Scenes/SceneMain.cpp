#include "SceneMain.h"

#include "DxLib.h"
#include "../Application.h"
#include "../Systems/input.h"
#include <cassert>
#include "../Game.h"

#include "../Utility/Collider.h"
#include "../Systems/Stage.h"
#include "../Utility/IntGraphDrawer.h"

#include "SceneManager.h"
#include "DebugScene.h"
#include "SceneClear.h"
#include "SceneGameOver.h"
#include "ScenePause.h"
#include "TutorialScene.h"

#include "../GameObjects/Player.h"

#include "../GameObjects/Enemies/Enemy.h"

#include "../GameObjects/ClearFlag.h"

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
#include "../Systems/SoundManager.h"

namespace
{
	// 画像リスト
	enum class Graphs
	{
		MapChip,
		ClearFlag,
		HpUI,
		BossHpUI,
		Bg,
		SubBg,
		ScoreText,
		NumberText,

		Num
	};

	// ロードする画像のファイル名
	const std::string kGraphFileNames[] =
	{
		"data/Map/MapChip.png",
		"data/ClearFlag.png",
		"data/UI/HpBar.png",
		"data/UI/BossHpBar.png",
		"data/Map/Bg.png",
		"data/Map/subBg.png",
		"data/UI/ScoreText.png",
		"data/UI/NumberText.png"
	};

	// Graphsで定義した画像数とkGraphFileNamesで定義したファイル名の要素数が一致していなかったらエラー
	constexpr int size = sizeof(kGraphFileNames) / sizeof(kGraphFileNames[0]);	// 配列全体のサイズ / 配列の要素一つのサイズ = 配列の要素数
	static_assert(static_cast<int>(Graphs::Num) == size);

	constexpr int kGoalFrame = 60;	// ゴール演出にかかるフレーム数
}

SceneMain::SceneMain(SceneManager& manager, Stages stage,int score) :
	SceneBase(manager),
	_frameCount(0),
	_nowStage(stage),
	_score(score)
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
		_manager.PushScene(std::make_shared<ScenePause>(_manager,_nowStage));
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
		//_manager.SetFadeCirclePos(_pPlayer->GetColliderPos() - _pCamera->GetDrawOffset());	// プレイヤーが死んだ位置にフェードサークルを置く
		// 全てのステージが未クリアなら再度チュートリアルステージを再生
		if (_manager.GetSaveData().clearedStage == static_cast<int>(Stages::None))
		{
			SoundManager::GetInstance().StopSound("TutorialBgm", true);
			_manager.ChangeSceneWithFade(std::make_shared<SceneMain>(_manager, _nowStage), FadeState::CircleFadeIn, FadeState::CircleFadeOut);
		}
		else	// そうでないならゲームオーバーへ
		{
			SoundManager::GetInstance().StopSound("StageBgm", true);
			SoundManager::GetInstance().StopSound("BossBgm", true);
			_manager.ChangeSceneWithFade(std::make_shared<SceneGameOver>(_manager, _nowStage,_score), FadeState::NormalFadeIn, FadeState::CircleFadeOut);
		}
		return;
	}

	// フェードサークルの位置更新
	Vector2 circleFadePos = _pPlayer->GetColliderPos() - _pCamera->GetDrawOffset();
	// プレイヤーの落下死でフェード位置が画面外になっている場合、マップの底面にフェード位置を設定する
	if (circleFadePos.y > _pMap->GetStageSize().y + 100)
	{
		circleFadePos.y = _pMap->GetStageSize().y - _pCamera->GetDrawOffset().y;
	}
	_manager.SetFadeCirclePos(circleFadePos);

	// 敵制御
	_pEnemyManager->Update();

	// ギミック
	_pGimmickManager->Update();

	// 弾制御
	_pBulletManager->Update(*_pMap, _pCamera->GetPos(), _pEnemyManager->GetEnemies(),_pGimmickManager->GetGimmicks());

	// アイテム制御
	_pItemManager->Update();

	// エフェクト制御
	_pEffectManager->Update();

	// HPUI更新
	_pHPUI->Update();
	if (_pBossHPUI != nullptr)
	{
		_pBossHPUI->Update();
	}

	// ゴールオブジェクト
	if (_pClearFlag != nullptr)
	{
		_pClearFlag->Update();
		if (_pPlayer->GetCollider()->CheckCollision(_pClearFlag->GetCollider()))
		{	// プレイヤーとゴール旗が接触したらゴール演出開始
			_goalAfterFrame++;
			_pPlayer->Goal(_pClearFlag->GetColliderPos());
		}
	}
	// ゴール後のフレーム数をカウントアップ
	if (_goalAfterFrame > 0) _goalAfterFrame++;
	// ゴール演出に要するフレーム経過したらステージクリア
	if (_goalAfterFrame > kGoalFrame) StageClear();

	if (_nowStage == Stages::Boss1 || 
		_nowStage == Stages::Boss2 ||
		_nowStage == Stages::Boss3 ||
		_nowStage == Stages::SecretBoss)	// ボスステージで敵がいなくなった(ボスを倒した)ならクリア
	{
		if (_pEnemyManager->GetEnemies().empty())
		{
			StageClear();
		}
	}

	// ステージ移動時のフェード処理
	if (_isClearFading)
	{	// フェードアウトが完了したら次のステージをロードしてフェードイン開始
		if (_manager.GetFadeState() == FadeState::NoFade)
		{
			LoadStage(static_cast<Stages>(static_cast<int>(_nowStage) + 1),_pPlayer->GetHp());
			_manager.SetFadeCirclePos(_pPlayer->GetPos() - _pCamera->GetDrawOffset());
			_manager.StartFadeIn(FadeState::CircleFadeIn);
			_isClearFading = false;
		}
	}

	// チュートリアルステージ用の更新処理
	if (_nowStage == Stages::Tutorial)
	{	
		if (TutorialUpdate())
		{
			return;	// チュートリアルシーンに切り替えたら以降の更新処理を行わない
		}
	}

	// 【デバッグ用】デバッグシーンに切り替え
#ifdef _DEBUG
	if (input.IsTriggered("select"))
	{
		_manager.ChangeScene(std::make_shared<DebugScene>(_manager));
	}
	if (CheckHitKey(KEY_INPUT_L))
	{
		StageClear();
	}
#endif
}

void SceneMain::Draw()
{
	// 背景の描画
	_pBg->Draw(_pCamera->GetDrawOffset());

	// マップの描画
	_pMap->Draw(_pCamera->GetDrawOffset());

	// ゴールの描画
	if (_pClearFlag != nullptr)
	{
		_pClearFlag->Draw(_pCamera->GetDrawOffset());
	}

	// ギミックの描画
	_pGimmickManager->Draw(_pCamera->GetDrawOffset());
	
	// 敵の描画
	_pEnemyManager->Draw();

	// 弾の描画
	_pBulletManager->Draw(_pCamera->GetDrawOffset());

	// アイテムの描画
	_pItemManager->Draw(_pCamera->GetDrawOffset());

	// エフェクトの描画
	_pEffectManager->Draw(_pCamera->GetDrawOffset());

	// プレイヤーの描画
	_pPlayer->Draw(_pCamera->GetDrawOffset());

	// UIの描画
	_pHPUI->Draw();
	if (_pBossHPUI != nullptr)
	{
		_pBossHPUI->Draw();
	}

	// スコアの描画
	DrawRotaGraph(130, 250, 0.4, 0.0, _graphHandles[static_cast<int>(Graphs::ScoreText)], true);
	IntGraphDrawer::Draw(210, 225, 0.4f, _graphHandles[static_cast<int>(Graphs::NumberText)], _score);


#ifdef _DEBUG
	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", _frameCount);
	DrawFormatString(0, 32, 0xffffff, "score:%d", _score);
#endif // _DEBUG
}

void SceneMain::LoadStage(Stages stage, int playerHp)
{
	// 現在のステージを設定
	_nowStage = stage;

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
		_pStage->LoadData("data/Stages/Stage1.fmf");
		break;
	case Stages::Boss1:
		_pStage->LoadData("data/Stages/Boss1Stage.fmf");
		break;
	case Stages::Stage2:
		_pStage->LoadData("data/Stages/Stage2.fmf");
		break;
	case Stages::Boss2:
		_pStage->LoadData("data/Stages/Boss2Stage.fmf");
		break;
	case Stages::Stage3:
		_pStage->LoadData("data/Stages/Stage3.fmf");
		break;
	case Stages::Boss3:
		_pStage->LoadData("data/Stages/Boss3Stage.fmf");
		break;
	case Stages::SecretStage:
		_pStage->LoadData("data/Stages/SecretStage.fmf");
		break;
	case Stages::SecretBoss:
		_pStage->LoadData("data/Stages/SecretBossStage.fmf");
		break;
	default:
		_pStage->LoadData("data/Stages/template.fmf");
		printfDx("未知のステージが呼ばれました");
		break;
	}

	// オブジェクトの生成、初期化
	
	// マップ
	_pMap = std::make_shared<Map>(_graphHandles[static_cast<int>(Graphs::MapChip)]);
	_pMap->SetMapData(_pStage->GetMapData(), _pStage->GetMapSize());		// マップデータを設定

	// カメラ
	_pCamera = std::make_shared<Camera>();
	_pCamera->SetStageSize(_pMap->GetStageSize());
	_pCamera->SetPos(Vector2(GlobalConstants::kScreenWidth / 2, _pMap->GetStageSize().y - GlobalConstants::kScreenHeight / 2));

	// エフェクト
	_pEffectManager = std::make_shared<EffectManager>();

	// 弾
	Vector2 mapSize = { static_cast<float>(_pStage->GetMapSize().w) * GlobalConstants::kDrawChipSize,static_cast<float>(_pStage->GetMapSize().h) * GlobalConstants::kDrawChipSize };
	_pBulletManager = std::make_shared<BulletManager>(mapSize);

	// プレイヤー
	bool isTutorial = (stage == Stages::Tutorial);
	if (isTutorial) playerHp = 4;
	_pPlayer = std::make_shared<Player>(*_pBulletManager,*_pEffectManager,isTutorial,playerHp);
	_pPlayer->InitPosFromStage(_pStage->GetObjectData(), _pStage->GetMapSize());		// プレイヤーの位置を設定
	_pBulletManager->SetPlayer(_pPlayer);	// プレイヤー情報を弾マネージャーに渡す

	// 背景
	_pBg = std::make_shared<Bg>(_graphHandles[static_cast<int>(Graphs::Bg)], _graphHandles[static_cast<int>(Graphs::SubBg)]);

	// ギミック
	_pGimmickManager = std::make_shared<GimmickManager>(_pPlayer);
	_pGimmickManager->LoadGimmicks(_pStage->GetObjectData(), _pStage->GetMapSize());// ギミックの生成

	// アイテム
	_pItemManager = std::make_shared<ItemManager>(_pPlayer, *this, *_pEffectManager);
	_pItemManager->LoadItems(_pStage->GetObjectData(), _pStage->GetMapSize());	// アイテムの生成

	// 敵
	_pEnemyManager = std::make_shared<EnemyManager>(_pPlayer, _pMap, _pCamera, _pGimmickManager, _pEffectManager, *_pItemManager, *_pBulletManager, *this, _manager);
	_pEnemyManager->LoadEnemies(_pStage->GetObjectData(), _pStage->GetMapSize());// 敵の生成

	// HPUI
	_pHPUI = std::make_shared<HPUI>(_graphHandles[static_cast<int>(Graphs::HpUI)], _pPlayer->GetMaxHp(), *_pPlayer, *_pMap, _pEnemyManager->GetEnemies());

	// ボスHPUI
	if (stage == Stages::Boss1 || 
		stage == Stages::Boss2 ||
		stage == Stages::Boss3 ||
		stage == Stages::SecretBoss)	// ボスステージならボスHPUIの生成
	{
		_pBossHPUI = std::make_shared<BossHPUI>(_graphHandles[static_cast<int>(Graphs::BossHpUI)], *_pPlayer,_pEnemyManager);
	}

	// ゴール旗を生成
	_pClearFlag = std::make_shared<ClearFlag>(Vector2(-10,-10), _pPlayer, _graphHandles[static_cast<int>(Graphs::ClearFlag)]);
	_pClearFlag->InitPosFromStage(_pStage->GetObjectData(), _pStage->GetMapSize());
	
	// BGM再生
	if (stage == Stages::Tutorial)
	{
		SoundManager::GetInstance().PlaySoundGame("TutorialBgm", true, true);
	}
	else if ( stage == Stages::Stage1 ||
		stage == Stages::Stage2 ||
		stage == Stages::Stage3 ||
		stage == Stages::SecretStage)
	{
		SoundManager::GetInstance().PlaySoundGame("StageBgm", true, true);
	}
	else if (stage == Stages::Boss1 ||
			 stage == Stages::Boss2 ||
			 stage == Stages::Boss3 ||
			 stage == Stages::SecretBoss)
	{
		SoundManager::GetInstance().PlaySoundGame("BossBgm", true, true);
	}
}

void SceneMain::StageClear()
{
	// 通常ステージかボスステージかを判定
	bool isNormalStage = _nowStage == Stages::Stage1 ||
		_nowStage == Stages::Stage2 ||
		_nowStage == Stages::Stage3 ||
		_nowStage == Stages::SecretStage;
	bool isBossStage = _nowStage == Stages::Boss1 ||
		_nowStage == Stages::Boss2 ||
		_nowStage == Stages::Boss3 ||
		_nowStage == Stages::SecretBoss;

	// 通常ステージならフェードアウトして次のボスステージへ
	if (isNormalStage)
	{
		SoundManager::GetInstance().StopSound("StageBgm", true);
		if (!_isClearFading)
		{
			_isClearFading = true;
			_manager.StartFadeOut(FadeState::CircleFadeOut);
		}
	}
	else if (isBossStage || _nowStage == Stages::Tutorial)	// ボスステージ、またはチュートリアルならクリアシーンへ
	{
		// bgmを停止する
		SoundManager::GetInstance().StopSound("TutorialBgm", true);
		SoundManager::GetInstance().StopSound("StageBgm", true);
		SoundManager::GetInstance().StopSound("BossBgm", true);
		_manager.ChangeSceneWithFade(std::make_shared<SceneClear>(_manager, _score, _nowStage), FadeState::NormalFadeIn, FadeState::CircleFadeOut);
	}
	else
	{
		printfDx("SceneMain::StageClearで未知のステージがクリアされました\n");
	}
	return;
}

bool SceneMain::TutorialUpdate()
{
	bool ans = false;
	const auto& playerPos = _pPlayer->GetPos();
	if (_nowTutorialStep == TutorialStep::None) _nowTutorialStep = TutorialStep::Move;
	switch (_nowTutorialStep)
	{
	case TutorialStep::None:
		TriggeredTutorial();
		ans = true;
		break;
	case TutorialStep::Move:
		TriggeredTutorial();
		ans = true;
		break;
	case TutorialStep::Shot:
		if (playerPos.x > 1300)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	case TutorialStep::Jump:
		if (playerPos.x > 1800)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	case TutorialStep::ChargeShot:
		if (playerPos.x > 2600)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	case TutorialStep::Dash:
		if (playerPos.x > 4100)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	case TutorialStep::WallJump:
		if ((playerPos.x > 4200 && playerPos.x < 4800 && playerPos.y > 1800) ||
			playerPos.x > 5900)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	case TutorialStep::DashInvincible:
		if (playerPos.x > 5200 && playerPos.y > 1700)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	case TutorialStep::JumpAndDash:
		if (playerPos.x > 6300)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	case TutorialStep::Complete:
		if (playerPos.x > 7600)
		{
			TriggeredTutorial();
			ans = true;
		}
		break;
	}
	return ans;
}

void SceneMain::TriggeredTutorial()
{
	// 現在のチュートリアルステップに応じてチュートリアルシーンを呼び出す
	_manager.PushScene(std::make_shared<TutorialScene>(_manager, _nowTutorialStep));
	// 次のチュートリアルステップへ進める
	if (_nowTutorialStep != TutorialStep::End)
	{
		_nowTutorialStep = static_cast<TutorialStep>(static_cast<int>(_nowTutorialStep) + 1);
	}
}
