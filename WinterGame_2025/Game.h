#pragma once

// ゲーム全体で使用する定数
namespace GlobalConstants
{
	// 画面情報
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;
	constexpr int kColorBitNum = 32;

	// パッド入力
	constexpr int kPadDown = 0x1;
	constexpr int kPadLeft = 0x2;
	constexpr int kPadRight = 0x4;
	constexpr int kPadUp = 0x8;

	constexpr int kPadA = 0x10;
	constexpr int kPadB = 0x20;
	constexpr int kPadX = 0x40;
	constexpr int kPadY = 0x80;

	constexpr int kPadLT = 0x100;
	constexpr int kPadRT = 0x200;
	constexpr int kPadBack = 0x400;
	constexpr int kPadStart = 0x800;

}