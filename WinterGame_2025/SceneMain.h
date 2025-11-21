#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

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

class Bg;

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
	void LoadAllGraphs();
	void DeleteAllGraphs();

private:
	/*画像ハンドル*/
	// プレイヤー関連
	int _playerH;
	int _playerWhiteH;
	int _playerShotH;
	int _chargeShotH;
	int _chargeParticleH;
	// 敵関連
	int _walkEnemyH;
	int _flyEnemyH;
	int _jumpEnemyH;
	// マップ関連
	int _mapChipH;
	// ギミック関連
	int _clearFlagH;
	int _laserH;
	// アイテム関連
	int _coinH;
	int _bigCoinH;
	int _healthItemH;
	// UI
	int _hpUIH;
	// 背景
	int _bgH;
	int _subBgH;

	int _frameCount;	// 経過フレーム数

	Stages _nowStage;

	/*オブジェクト*/
	std::shared_ptr<Player> _pPlayer;	// プレイヤー
	std::vector<std::shared_ptr<Bullet>> _pBullets;	// 弾
	std::vector<std::shared_ptr<Enemy>> _pEnemies;	// 敵
	std::vector<std::shared_ptr<Item>> _pItems;	// アイテム
	std::vector<std::shared_ptr<Gimmick>> _pGimmicks;	// ギミック
	std::shared_ptr<ClearFlag> _pClearFlag;	// ゴール旗
	std::shared_ptr<Map> _pMap;	// マップ
	std::shared_ptr<Camera> _pCamera;	// カメラ
	/*UIオブジェクト*/
	std::shared_ptr<HPUI> _pHPUI;

	std::shared_ptr<Bg> _pBg;
};

