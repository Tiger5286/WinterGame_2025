#pragma once

// ゲーム全体で使用する定数
namespace GlobalConstants
{
	// 画面情報
	constexpr int SCREEN_WIDTH = 1920;
	constexpr int SCREEN_HEIGHT = 1080;
	constexpr int COLOR_BIT_NUM = 32;

	// パッド入力
	constexpr int PAD_DOWN = 0x1;
	constexpr int PAD_LEFT = 0x2;
	constexpr int PAD_RIGHT = 0x4;
	constexpr int PAD_UP = 0x8;

	constexpr int PAD_A = 0x10;
	constexpr int PAD_B = 0x20;
	constexpr int PAD_X = 0x40;
	constexpr int PAD_Y = 0x80;

	constexpr int PAD_LT = 0x100;
	constexpr int PAD_RT = 0x200;
	constexpr int PAD_SELECT = 0x400;
	constexpr int PAD_START = 0x800;
}