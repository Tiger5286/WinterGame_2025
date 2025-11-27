#include "HPUI.h"
#include "Dxlib.h"
#include <cmath>

#include "../GameObjects/Enemies/Enemy.h"

namespace
{
	constexpr int kFrameLeft = 160;
	constexpr int kFrameTop = 90;
	constexpr int kFrameRight = 520;
	constexpr int kFrameBottom = 170;

	constexpr int kBarLeft = kFrameLeft + 10;
	constexpr int kBarTop = kFrameTop + 10;
	constexpr int kBarRight = kFrameRight - 10;
	constexpr int kBarBottom = kFrameBottom - 10;

	constexpr int kPosX = 300;
	constexpr int kPosY = 130;

	constexpr int kLowAlphaDis = 150;

	constexpr int kMaxBarLength = kBarRight - kBarLeft;
}

HPUI::HPUI(int handle,int playerMaxHp) :
	_handle(handle),
	_playerMaxHp(playerMaxHp),
	_barLength(kMaxBarLength),
	_drawBarLength(kMaxBarLength),
	_alpha(255)
{
}

HPUI::~HPUI()
{
}

void HPUI::Init()
{
}

void HPUI::Update(int playerHP)
{
	// プレイヤーのhpからバーの長さを出す
	_barLength = static_cast<float>(playerHP) / static_cast<float>(_playerMaxHp) * kMaxBarLength;
	// lerpでいい感じに減らす
	_drawBarLength = std::lerp(_drawBarLength, _barLength, 0.05f);
}

void HPUI::Draw(Vector2 drawPlayerPos,const std::vector<std::shared_ptr<Enemy>>& pEnemys)
{


	// プレイヤーがUIの近くにいるときは透明にする
	bool isPlayerNear = drawPlayerPos.y < kFrameBottom + kLowAlphaDis && drawPlayerPos.x < kFrameRight + kLowAlphaDis;
	// 敵がUIの近くにいるときは透明にする
	bool isEnemyNear = false;
	for (const auto& enemy : pEnemys)
	{
		Vector2 enemyPos = enemy->GetPos();
		if (enemyPos.y < kFrameBottom + kLowAlphaDis && enemyPos.x < kFrameRight + kLowAlphaDis)
		{
			isEnemyNear = true;
			break;
		}
	}
	// 透明度をlerpでいい感じに変える
	if (isPlayerNear || isEnemyNear)
	{
		_alpha = std::lerp(_alpha, 64, 0.2f);
	}
	else
	{
		_alpha = std::lerp(_alpha, 255, 0.2f);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	//DrawBox(FRAME_LEFT, FRAME_TOP, FRAME_RIGHT, FRAME_BOTTOM, 0x888888, true);	// 枠
	DrawBox(kBarLeft + _drawBarLength, kBarTop, kBarRight, kBarBottom, 0x000000, true);	// HPないとこの黒
	DrawBox(kBarLeft + _barLength, kBarTop, kBarLeft + _drawBarLength, kBarBottom, 0xff0000, true);	// HP減る量の赤
	DrawBox(kBarLeft, kBarTop, kBarLeft + _barLength, kBarBottom, 0xffff00, true);	// HPあるとこの黄色
	DrawRotaGraph(kPosX, kPosY, 0.4, 0.0, _handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}