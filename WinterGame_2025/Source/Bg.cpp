#include "Bg.h"
#include "Game.h"
#include "Dxlib.h"

namespace
{
	constexpr int kScrollDecayBg = 3;	// スクロール減衰量(普通よりスクロールする量を減らして遠くにあるように見せる)
	constexpr int kScrollDecaySubBg = 2;
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
	int drawIndexX = (drawOffset.x / kScrollDecayBg) / GlobalConstants::kScreenWidth;
	int drawIndexY = (drawOffset.y / kScrollDecayBg) / GlobalConstants::kScreenHeight;
	int x = drawOffset.x / kScrollDecayBg;
	int y = drawOffset.y / kScrollDecayBg;

	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 0) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		_bgH, false);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 0) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 2) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		_bgH, false);
	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 2) * GlobalConstants::kScreenHeight - y,
		_bgH, false);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 2) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 2) * GlobalConstants::kScreenHeight - y,
		_bgH, false);
}

void Bg::DrawSubBg(Vector2 drawOffset)
{
	int drawIndexX = (drawOffset.x / kScrollDecaySubBg) / GlobalConstants::kScreenWidth;
	int drawIndexY = (drawOffset.y / kScrollDecaySubBg) / GlobalConstants::kScreenHeight;
	int x = drawOffset.x / kScrollDecaySubBg;
	int y = drawOffset.y / kScrollDecaySubBg;

	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 0) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		_subBgH, true);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 0) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 2) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		_subBgH, true);
	DrawExtendGraph((drawIndexX + 0) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 2) * GlobalConstants::kScreenHeight - y,
		_subBgH, true);
	DrawExtendGraph((drawIndexX + 1) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 1) * GlobalConstants::kScreenHeight - y,
		(drawIndexX + 2) * GlobalConstants::kScreenWidth - x,
		(drawIndexY + 2) * GlobalConstants::kScreenHeight - y,
		_subBgH, true);
}
