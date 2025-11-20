#include "Bg.h"
#include "Game.h"
#include "Dxlib.h"

namespace
{
	constexpr int SCROLL_DECAY = 3;	// スクロール減衰量(普通よりスクロールする量を減らして遠くにあるように見せる)
}

Bg::Bg(int handle):
	_handle(handle)
{
}

Bg::~Bg()
{
}

void Bg::Init()
{
}

void Bg::Update()
{
}

void Bg::Draw(Vector2 drawOffset)
{
	int drawIndexX = (drawOffset.x / SCROLL_DECAY) / GlobalConstants::SCREEN_WIDTH;
	int drawIndexY = (drawOffset.y / SCROLL_DECAY) / GlobalConstants::SCREEN_HEIGHT;
	int x = drawOffset.x / SCROLL_DECAY;
	int y = drawOffset.y / SCROLL_DECAY;

	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 0) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		_handle, false);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 0) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 2) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		_handle, false);

	//if (static_cast<int>(drawOffset.x) % GlobalConstants::SCREEN_WIDTH > 0)
	//{
	//	DrawExtendGraph(0 - drawOffset.x / SCROLL_DECAY + GlobalConstants::SCREEN_WIDTH,
	//		0 - drawOffset.y / SCROLL_DECAY,
	//		GlobalConstants::SCREEN_WIDTH - drawOffset.x / SCROLL_DECAY + GlobalConstants::SCREEN_WIDTH,
	//		GlobalConstants::SCREEN_HEIGHT - drawOffset.y / SCROLL_DECAY,
	//		_handle, false);
	//}


	DrawFormatString(100, 200, 0xffffff, "%.2f,%.2f", drawOffset.x / 3, drawOffset.y / 3);
	DrawFormatString(100, 216, 0xffffff, "%d,%d", drawIndexX, drawIndexY);
}