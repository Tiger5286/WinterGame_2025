#include "IntGraphDrawer.h"
#include "Dxlib.h"
#include <cassert>

namespace
{
	constexpr int kGraphSize = 130;
}

IntGraphDrawer::IntGraphDrawer()
{
	_handle = LoadGraph("data/UI/NumberText.png");
	assert(_handle != -1);
}

IntGraphDrawer::~IntGraphDrawer()
{
	DeleteGraph(_handle);
}

void IntGraphDrawer::Draw(int value)
{
	int digits = CountDigits(value);
	for (int i = digits; i > 0; i--)
	{
		DrawRectRotaGraph(kGraphSize / 2, kGraphSize / 2, 0, 0, kGraphSize, kGraphSize, 0.0, 0.0, _handle, true);
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
