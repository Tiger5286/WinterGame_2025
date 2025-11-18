#include "HPUI.h"
#include "Dxlib.h"
#include <cmath>

namespace
{
	constexpr int FRAME_LEFT = 50;
	constexpr int FRAME_TOP = 50;
	constexpr int FRAME_RIGHT = 450;
	constexpr int FRAME_BOTTOM = 150;

	constexpr int BAR_LEFT = FRAME_LEFT + 10;
	constexpr int BAR_TOP = FRAME_TOP + 10;
	constexpr int BAR_RIGHT = FRAME_RIGHT - 10;
	constexpr int BAR_BOTTOM = FRAME_BOTTOM - 10;

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
	_barLength = static_cast<float>(playerHP) / static_cast<float>(_playerMaxHp) * MAX_BAR_LENGTH;
	_drawBarLength = std::lerp(_drawBarLength, _barLength, 0.05f);


}

void HPUI::Draw(Vector2 drawPlayerPos)
{
	//if (drawPlayerPos.y < FRAME_BOTTOM + LOW_ALPHA_DIS && drawPlayerPos.x < FRAME_RIGHT + LOW_ALPHA_DIS)
	//{
	//	_alpha = std::lerp(_alpha, 64, 0.2f);
	//}
	//else
	//{
	//	_alpha = std::lerp(_alpha, 255, 0.2f);
	//}
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	//DrawBox(FRAME_LEFT, FRAME_TOP, FRAME_RIGHT, FRAME_BOTTOM, 0x888888, true);	// ˜g
	//DrawBox(BAR_LEFT + _drawBarLength, BAR_TOP, BAR_RIGHT, BAR_BOTTOM, 0x000000, true);	// HP‚È‚¢‚Æ‚±‚Ì•
	//DrawBox(BAR_LEFT + _barLength, BAR_TOP, BAR_LEFT + _drawBarLength, BAR_BOTTOM, 0xff0000, true);	// HPŒ¸‚é—Ê‚ÌÔ
	//DrawBox(BAR_LEFT, BAR_TOP, BAR_LEFT + _barLength, BAR_BOTTOM, 0xffff00, true);	// HP‚ ‚é‚Æ‚±‚Ì‰©F
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	DrawGraph(FRAME_LEFT, FRAME_TOP, _handle, true);
}