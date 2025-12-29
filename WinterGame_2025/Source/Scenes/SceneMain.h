#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../Game.h"

class Stage;

class Input;

class Player;
class Bullet;

class ClearFlag;

class Map;
class Camera;

class HPUI;
class BossHPUI;

class Bg;

class Fade;

class EnemyManager;
class GimmickManager;
class ItemManager;
class BulletManager;
class EffectManager;

enum class TutorialStep
{
	None,
	Move,
	Shot,
	Jump,
	ChargeShot,
	Dash,
	DashInvincible,
	WallJump,
	JumpAndDash,
	Complete,
	End
};

class SceneMain : public SceneBase
{
public:
	SceneMain(SceneManager& manager, Stages stage,int score = 0);
	~SceneMain();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;

	/// <summary>
	/// ステージを読み込む
	/// </summary>
	/// <param name="stage">読み込むステージ</param>
	void LoadStage(Stages stage,int playerHp = 5);

	/// <summary>
	/// スコアを加算する
	/// </summary>
	/// <param name="score">加算するスコア</param>
	void AddScore(int score) { _score += score; }

private:

	/// <summary>
	/// ステージをクリアしたときの処理
	/// </summary>
	void StageClear();

	/// <summary>
	/// チュートリアルステージ用の更新処理
	/// </summary>
	/// <returns>チュートリアルシーンを生成したかどうか</returns>
	bool TutorialUpdate();

	/// <summary>
	/// チュートリアルシーンを呼び出す
	/// </summary>
	/// <returns>チュートリアルシーンを生成したかどうか</returns>
	void TriggeredTutorial();

private:
	/*画像ハンドル*/
	std::vector<int> _graphHandles;

	int _frameCount;	// 経過フレーム数
	int _score = 0;		// スコア

	bool _isClearFading = false;	// ステージクリア時のフェードを行っているかどうか

	Stages _nowStage;	// 今プレイしているステージ

	// ステージを読み込むためのクラスを準備
	std::shared_ptr<Stage> _pStage;

	/*オブジェクト*/
	std::shared_ptr<Player> _pPlayer;	// プレイヤー
	std::shared_ptr<ClearFlag> _pClearFlag;	// ゴール旗
	std::shared_ptr<Map> _pMap;	// マップ
	std::shared_ptr<Camera> _pCamera;	// カメラ
	/*UIオブジェクト*/
	std::shared_ptr<HPUI> _pHPUI;
	std::shared_ptr<BossHPUI> _pBossHPUI;
	// 背景オブジェクト
	std::shared_ptr<Bg> _pBg;
	// 各種Manager
	std::shared_ptr<EnemyManager> _pEnemyManager;
	std::shared_ptr<GimmickManager> _pGimmickManager;
	std::shared_ptr<ItemManager> _pItemManager;
	std::shared_ptr<BulletManager> _pBulletManager;
	std::shared_ptr<EffectManager> _pEffectManager;

	// チュートリアル用
	TutorialStep _nowTutorialStep = TutorialStep::None;
};

