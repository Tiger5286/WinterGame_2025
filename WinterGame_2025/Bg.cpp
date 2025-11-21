#include "Bg.h"
#include "Game.h"
#include "Dxlib.h"

namespace
{
	constexpr int SCROLL_DECAY_BG = 3;	// スクロール減衰量(普通よりスクロールする量を減らして遠くにあるように見せる)
	constexpr int SCROLL_DECAY_SUBBG = 2;
}

Bg::Bg(int bgH,int subBgH):
	_bgH(bgH),
	_subBgH(subBgH)
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
	DrawBg(drawOffset);
	DrawSubBg(drawOffset);

	//DrawFormatString(100, 200, 0xffffff, "%.2f,%.2f", drawOffset.x / 3, drawOffset.y / 3);
	//DrawFormatString(100, 216, 0xffffff, "%d,%d", drawIndexX, drawIndexY);
}

void Bg::DrawBg(Vector2 drawOffset)
{
	int drawIndexX = (drawOffset.x / SCROLL_DECAY_BG) / GlobalConstants::SCREEN_WIDTH;
	int drawIndexY = (drawOffset.y / SCROLL_DECAY_BG) / GlobalConstants::SCREEN_HEIGHT;
	int x = drawOffset.x / SCROLL_DECAY_BG;
	int y = drawOffset.y / SCROLL_DECAY_BG;

	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 0) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		_bgH, false);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 0) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 2) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		_bgH, false);
	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 2) * GlobalConstants::SCREEN_HEIGHT - y,
		_bgH, false);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 2) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 2) * GlobalConstants::SCREEN_HEIGHT - y,
		_bgH, false);
}

void Bg::DrawSubBg(Vector2 drawOffset)
{
	int drawIndexX = (drawOffset.x / SCROLL_DECAY_SUBBG) / GlobalConstants::SCREEN_WIDTH;
	int drawIndexY = (drawOffset.y / SCROLL_DECAY_SUBBG) / GlobalConstants::SCREEN_HEIGHT;
	int x = drawOffset.x / SCROLL_DECAY_SUBBG;
	int y = drawOffset.y / SCROLL_DECAY_SUBBG;

	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 0) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		_subBgH, true);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 0) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 2) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		_subBgH, true);
	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 2) * GlobalConstants::SCREEN_HEIGHT - y,
		_subBgH, true);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 1) * GlobalConstants::SCREEN_HEIGHT - y,
		(drawIndexX + 2) * GlobalConstants::SCREEN_WIDTH - x,
		(drawIndexY + 2) * GlobalConstants::SCREEN_HEIGHT - y,
		_subBgH, true);
}
