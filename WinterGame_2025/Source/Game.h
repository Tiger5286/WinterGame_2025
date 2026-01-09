#pragma once
#include <array>

// ゲーム全体で使用する定数
namespace GlobalConstants
{
	// 画面情報
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;
	constexpr int kColorBitNum = 32;

	// パッド入力
	constexpr int kPadInputDown = 0x1;
	constexpr int kPadInputLeft = 0x2;
	constexpr int kPadInputRight = 0x4;
	constexpr int kPadInputUp = 0x8;

	constexpr int kPadInputA = 0x10;
	constexpr int kPadInputB = 0x20;
	constexpr int kPadInputX = 0x40;
	constexpr int kPadInputY = 0x80;

	constexpr int kPadInputLT = 0x100;
	constexpr int kPadInputRT = 0x200;
	constexpr int kPadInputSelect = 0x400;
	constexpr int kPadInputStart = 0x800;

	// マップチップ情報
	constexpr int kChipSize = 16;
	constexpr float kDrawScale = 3.0f;
	constexpr int kDrawChipSize = static_cast<int>(kChipSize * kDrawScale);
	constexpr int kDrawChipSizeHalf = static_cast<int>(kChipSize * kDrawScale / 2);

	// 共通フォント名
	constexpr char kMainFontName[] = "廻想体 ネクスト UP B";
}

// オブジェクトデータ定義
enum class ObjectData : int
{
	None,
	PlayerSpawn,
	Coin,
	BigCoin,
	HealthItem,
	Spike,
	Blank1,
	ClearFlag,
	DownLaser2,
	DownLaser3,
	DownLaser4,
	DownLaser5,
	DownLaser6,
	DownLaser7,
	DownLaser8,
	DownLaser9,
	UpLaser2,
	UpLaser3,
	UpLaser4,
	UpLaser5,
	UpLaser6,
	UpLaser7,
	UpLaser8,
	UpLaser9,
	WalkEnemyIdle,
	WalkEnemyMoveLeft,
	WalkEnemyMoveRight,
	FlyEnemyIdle,
	FlyEnemyMove,
	JumpEnemy,
	DroneEnemy,

	BossEnemy1 = 56,
	BossEnemy2,
	BossEnemy3,
};

// ステージの種類定義
enum class Stages
{
	None,
	Tutorial,
	Stage1,
	Boss1,
	Stage2,
	Boss2,
	Stage3,
	Boss3,
	SecretStage,
	SecretBoss,

	Num,
	Temp,
};

// 選択可能なステージ定義
enum class SelectableStages
{
	None,
	Tutorial,
	Stage1,
	Stage2,
	Stage3,
	SecretStage,
	Num,
};

/// <summary>
/// ステージを選択可能なステージに変換する
/// </summary>
/// <param name="stage">ステージ</param>
/// <returns>選択可能ステージ</returns>
static SelectableStages StageToSelectableStage(Stages stage)
{
	switch (stage)
	{
	case Stages::Tutorial:
		return SelectableStages::Tutorial;
	case Stages::Stage1:
	case Stages::Boss1:
		return SelectableStages::Stage1;
	case Stages::Stage2:
	case Stages::Boss2:
		return SelectableStages::Stage2;
	case Stages::Stage3:
	case Stages::Boss3:
		return SelectableStages::Stage3;
	case Stages::SecretStage:
	case Stages::SecretBoss:
		return SelectableStages::SecretStage;
	default:
		return SelectableStages::None;
	}
}

/// <summary>
/// 選択可能ステージをステージに変換する
/// </summary>
/// <param name="stage">選択可能ステージ</param>
/// <returns>ステージ</returns>
static Stages SelectableStageToStage(SelectableStages stage)
{
	switch (stage)
	{
	case SelectableStages::Tutorial:
		return Stages::Tutorial;
	case SelectableStages::Stage1:
		return Stages::Stage1;
	case SelectableStages::Stage2:
		return Stages::Stage2;
	case SelectableStages::Stage3:
		return Stages::Stage3;
	case SelectableStages::SecretStage:
		return Stages::SecretStage;
	default:
		return Stages::None;
	}
}

struct SaveDataHeader
{
	char signature[4];	// "sdat"
	float version;
	int size;
};

struct SaveData
{
	int clearedStage = 0;
	bool isReleasedSecretStage = false;
	std::array<int, static_cast<int>(SelectableStages::Num)> highScores = {0};

	int bgmVolume = 255;
	int seVolume = 255;
};