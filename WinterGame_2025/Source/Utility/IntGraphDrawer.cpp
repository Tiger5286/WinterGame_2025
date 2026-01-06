#include "IntGraphDrawer.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	constexpr int kGraphSize = 130;
}

IntGraphDrawer::IntGraphDrawer()
{
}

IntGraphDrawer::~IntGraphDrawer()
{
}

void IntGraphDrawer::Draw(int x, int y, float scale, int handle, int value)
{
	int digits = CountDigits(value);
	for (int i = 0; i < digits; i++)
	{
		int nowDigit = digits - i;
		int digitValue = PickOutDigit(value, nowDigit);

		DrawRectRotaGraph(x + (kGraphSize / 2) * scale + i * (kGraphSize * scale),
			y + (kGraphSize / 2) * scale,
			0 + digitValue*kGraphSize, 0,
			kGraphSize, kGraphSize,
			scale, 0.0, handle, true);
	}
}

int IntGraphDrawer::CountDigits(int value)
{
	if (value == 0) return 1;
	value = abs(value);

	int ans = 0;
	while (value > 0)
	{
		value /= 10;
		ans++;
	}
	return ans;
}

int IntGraphDrawer::PickOutDigit(int value, int digit)
{
	// 無効な場合をはじく
	if (digit <= 0) return 0;
	if (CountDigits(value) < digit) return 0;
	// 正の数に変換
	value = abs(value);
	// 10のdigit乗を計算
	int pow10 = 1;
	for (int i = 0; i < digit - 1; i++)
	{
		pow10 *= 10;
	}
	// 指定された桁の数値を取り出して返す
	return (value / pow10) % 10;
}
