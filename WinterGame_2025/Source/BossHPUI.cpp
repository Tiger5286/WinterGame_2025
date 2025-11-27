#include "BossHPUI.h"
#include <cmath>
#include "DxLib.h"
#include "GameObjects/Enemy.h"

namespace
{
	constexpr int kBarLeft = 975;
	constexpr int kBarTop = 100;
	constexpr int kBarRight = 1770;
	constexpr int kBarBottom = 160;

	constexpr int kPosX = 1400;
	constexpr int kPosY = 130;

	constexpr int kLowAlphaDis = 150;

	constexpr int kMaxBarLength = kBarRight - kBarLeft;
}

BossHPUI::BossHPUI(int handle, int bossMaxHp) :
	_handle(handle),
	_bossMaxHp(bossMaxHp),
	_barLength(0),
	_drawBarLength(0),
	_alpha(255)
{
}

BossHPUI::~BossHPUI()
{
}

void BossHPUI::Init()
{
}

void BossHPUI::Update(int bossHp)
{
	// ボスのhpからバーの長さを出す
	_barLength = static_cast<float>(bossHp) / static_cast<float>(_bossMaxHp) * (kBarRight - kBarLeft);
	// lerpでいい感じに減らす
	_drawBarLength = std::lerp(_drawBarLength, _barLength, 0.05f);
}

void BossHPUI::Draw(Vector2 drawPlayerPos, const std::vector<std::shared_ptr<Enemy>>& pEnemys)
{
	// プレイヤーがUIの近くにいるときは透明にする
	bool isPlayerNear = drawPlayerPos.y < kBarBottom + kLowAlphaDis && drawPlayerPos.x < kBarRight + kLowAlphaDis;
	// 敵がUIの近くにいるときは透明にする
	bool isEnemyNear = false;
	for (const auto& enemy : pEnemys)
	{
		Vector2 enemyPos = enemy->GetPos();
		if (enemyPos.y < kBarBottom + kLowAlphaDis && enemyPos.x < kBarRight + kLowAlphaDis)
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