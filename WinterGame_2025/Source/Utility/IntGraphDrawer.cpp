#include "IntGraphDrawer.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	constexpr int kGraphSize = 130;
}

void IntGraphDrawer::Draw(int x, int y, float scale, int handle, int value,int drawDigits)
{
	// Œ…”‚ğŒvZ
	int digits = CountDigits(value);
	// ŠeŒ…‚ğ•`‰æ
	for (int i = 0; i < drawDigits; i++)
	{
		// •`‰æ‚·‚éŒ…
		int nowDigit = drawDigits - i;

		// •`‰æ‚·‚éŒ…‚ªÀÛ‚ÌŒ…”‚ğ’´‚¦‚Ä‚¢‚éê‡‚Í0‚ğ•`‰æ
		int digitValue = 0;
		if (!(nowDigit > digits))
		{
			// æ‚èo‚µ‚½Œ…‚Ì”’l‚ğæ“¾
			digitValue = PickOutDigit(value, nowDigit);
		}

		// •`‰æ
		DrawRectRotaGraph(static_cast<int>(x + (kGraphSize / 2) * scale + i * (kGraphSize * scale)),
			static_cast<int>(y + (kGraphSize / 2) * scale),
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
	// –³Œø‚Èê‡‚ğ‚Í‚¶‚­
	if (digit <= 0) return 0;
	if (CountDigits(value) < digit) return 0;
	// ³‚Ì”‚É•ÏŠ·
	value = abs(value);
	// 10‚Ìdigitæ‚ğŒvZ
	int pow10 = 1;
	for (int i = 0; i < digit - 1; i++)
	{
		pow10 *= 10;
	}
	// w’è‚³‚ê‚½Œ…‚Ì”’l‚ğæ‚èo‚µ‚Ä•Ô‚·
	return (value / pow10) % 10;
}
