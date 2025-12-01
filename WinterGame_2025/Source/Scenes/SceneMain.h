#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <string>

class Stage;

class Input;

class Player;
class Bullet;

class Item;

class Enemy;
class Gimmick;

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

private:
	/*画像ハンドル*/
	std::vector<int> _graphHandles;

	int _frameCount;	// 経過フレーム数

	Stages _nowStage;

	std::shared_ptr<Stage> _pStage;

	/*オブジェクト*/
	std::shared_ptr<Player> _pPlayer;	// プレイヤー
	std::vector<std::shared_ptr<Bullet>> _pBullets;	// 弾
	std::shared_ptr<ClearFlag> _pClearFlag;	// ゴール旗
	std::shared_ptr<Map> _pMap;	// マップ
	std::shared_ptr<Camera> _pCamera;	// カメラ
	/*UIオブジェクト*/
	std::shared_ptr<HPUI> _pHPUI;
	std::shared_ptr<BossHPUI> _pBossHPUI;

	std::shared_ptr<Bg> _pBg;

	std::shared_ptr<EnemyManager> _pEnemyManager;
	std::shared_ptr<GimmickManager> _pGimmickManager;
	std::shared_ptr<ItemManager> _pItemManager;
};

