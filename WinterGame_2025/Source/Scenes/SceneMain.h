#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <string>

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

// どのステージかを表す列挙体
enum class Stages
{
	Temp,
	Tutorial,
	Stage1,
	Stage2,
	Boss
};

class SceneMain : public SceneBase
{
public:
	SceneMain(SceneManager& manager, Stages stage);
	~SceneMain();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;

	void LoadStage(Stages stage);

	void StageClear();

	void AddScore(int score) { _score += score; }

private:
	/*画像ハンドル*/
	std::vector<int> _graphHandles;

	int _frameCount;	// 経過フレーム数
	int _score = 0;		// スコア

	Stages _nowStage;

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
};

