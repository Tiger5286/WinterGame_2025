#include "HPUI.h"
#include "Dxlib.h"
#include <cmath>

#include "Enemy.h"

namespace
{
	constexpr int FRAME_LEFT = 160;
	constexpr int FRAME_TOP = 90;
	constexpr int FRAME_RIGHT = 520;
	constexpr int FRAME_BOTTOM = 170;

	constexpr int BAR_LEFT = FRAME_LEFT + 10;
	constexpr int BAR_TOP = FRAME_TOP + 10;
	constexpr int BAR_RIGHT = FRAME_RIGHT - 10;
	constexpr int BAR_BOTTOM = FRAME_BOTTOM - 10;

	constexpr int POS_X = 300;
	constexpr int POS_Y = 130;

	constexpr int LOW_ALPHA_DIS = 150;

	constexpr int MAX_BAR_LENGTH = BAR_RIGHT - BAR_LEFT;
}

HPUI::HPUI(int handle,int playerMaxHp) :
	_handle(handle),
	_playerMaxHp(playerMaxHp),
	_barLength(MAX_BAR_LENGTH),
	_drawBarLength(MAX_BAR_LENGTH),
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
	_barLength = static_cast<float>(playerHP) / static_cast<float>(_playerMaxHp) * MAX_BAR_LENGTH;
	// lerpでいい感じに減らす
	_drawBarLength = std::lerp(_drawBarLength, _barLength, 0.05f);
}

void HPUI::Draw(Vector2 drawPlayerPos,const std::vector<std::shared_ptr<Enemy>>& pEnemys)
{


	// プレイヤーがUIの近くにいるときは透明にする
	bool isPlayerNear = drawPlayerPos.y < FRAME_BOTTOM + LOW_ALPHA_DIS && drawPlayerPos.x < FRAME_RIGHT + LOW_ALPHA_DIS;
	// 敵がUIの近くにいるときは透明にする
	bool isEnemyNear = false;
	for (const auto& enemy : pEnemys)
	{
		Vector2 enemyPos = enemy->GetPos();
		if (enemyPos.y < FRAME_BOTTOM + LOW_ALPHA_DIS && enemyPos.x < FRAME_RIGHT + LOW_ALPHA_DIS)
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