#pragma once

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
	constexpr int kDrawChipSize = kChipSize * kDrawScale;
	constexpr int kDrawChipSizeHalf = kChipSize * kDrawScale / 2;
}

// オブジェクトデータ定義
enum class ObjectData : int
{
	None,
	PlayerSpawn,
	Coin,
	BigCoin,
	HealthItem,
	Blank0,
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
};