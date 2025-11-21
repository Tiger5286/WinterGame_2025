#include "BossHPUI.h"
#include <cmath>
#include "DxLib.h"
#include "Enemy.h"

namespace
{
	constexpr int BAR_LEFT = 950;
	constexpr int BAR_TOP = 100;
	constexpr int BAR_RIGHT = 1770;
	constexpr int BAR_BOTTOM = 160;

	constexpr int POS_X = 1400;
	constexpr int POS_Y = 130;

	constexpr int LOW_ALPHA_DIS = 150;

	constexpr int MAX_BAR_LENGTH = BAR_RIGHT - BAR_LEFT;
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
	_barLength = static_cast<float>(bossHp) / static_cast<float>(_bossMaxHp) * (BAR_RIGHT - BAR_LEFT);
	// lerpでいい感じに減らす
	_drawBarLength = std::lerp(_drawBarLength, _barLength, 0.05f);
}

void BossHPUI::Draw(Vector2 drawPlayerPos, const std::vector<std::shared_ptr<Enemy>>& pEnemys)
{
	// プレイヤーがUIの近くにいるときは透明にする
	bool isPlayerNear = drawPlayerPos.y < BAR_BOTTOM + LOW_ALPHA_DIS && drawPlayerPos.x < BAR_RIGHT + LOW_ALPHA_DIS;
	// 敵がUIの近くにいるときは透明にする
	bool isEnemyNear = false;
	for (const auto& enemy : pEnemys)
	{
		Vector2 enemyPos = enemy->GetPos();
		if (enemyPos.y < BAR_BOTTOM + LOW_ALPHA_DIS && enemyPos.x < BAR_RIGHT + LOW_ALPHA_DIS)
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
	DrawBox(BAR_LEFT + _drawBarLength, BAR_TOP, BAR_RIGHT, BAR_BOTTOM, 0x000000, true);	// HPないとこの黒
	DrawBox(BAR_LEFT + _barLength, BAR_TOP, BAR_LEFT + _drawBarLength, BAR_BOTTOM, 0xff0000, true);	// HP減る量の赤
	DrawBox(BAR_LEFT, BAR_TOP, BAR_LEFT + _barLength, BAR_BOTTOM, 0xffff00, true);	// HPあるとこの黄色
	DrawRotaGraph(POS_X, POS_Y, 0.4, 0.0, _handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}