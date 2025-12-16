#include "HPUI.h"
#include "Dxlib.h"
#include <cmath>

#include "../GameObjects/Player.h"
#include "../GameObjects/Enemies/Enemy.h"

namespace
{
	// UI画像の情報
	constexpr int kGraphSizeW = 1300;
	constexpr int kGraphSizeH = 500;
	// UIのバーの情報
	constexpr int kBarL = 335;
	constexpr int kBarR = 1160;
	constexpr int kBarT = 187;
	constexpr int kBarB = 318;
	// UIの描画情報
	constexpr float kDrawScale = 0.4f;
	constexpr int kDrawPosX = 300;
	constexpr int kDrawPosY = 130;
	constexpr int kDrawPosL = kDrawPosX - (kGraphSizeW * kDrawScale) / 2;
	constexpr int kDrawPosT = kDrawPosY - (kGraphSizeH * kDrawScale) / 2;
	// バーの描画情報
	constexpr int kDrawBarL = kDrawPosL + (kBarL * kDrawScale);
	constexpr int kDrawBarR = kDrawPosL + (kBarR * kDrawScale);
	constexpr int kDrawBarT = kDrawPosT + (kBarT * kDrawScale);
	constexpr int kDrawBarB = kDrawPosT + (kBarB * kDrawScale) + 1;	// 隙間を埋めるために1足す
	constexpr int kMaxBarLength = kDrawBarR - kDrawBarL;

	// 透明になる領域を余分に確保する量
	constexpr int kLowAlphaDis = 100;
}

HPUI::HPUI(int handle,const int playerMaxHp, const Player& player, const std::vector<std::shared_ptr<Enemy>>& pEnemies) :
	_handle(handle),
	_playerMaxHp(playerMaxHp),
	_barLength(kMaxBarLength),
	_drawBarLength(kMaxBarLength),
	_alpha(255),
	_player(player),
	_pEnemies(pEnemies)
{
}

HPUI::~HPUI()
{
}

void HPUI::Init()
{
}

void HPUI::Update()
{
	// プレイヤーのhpからバーの長さを出す
	_barLength = static_cast<float>(_player.GetHp()) / static_cast<float>(_playerMaxHp) * kMaxBarLength;
	// バーの長さを徐々に変える
	if (_drawBarLength > _barLength)
	{
		_drawBarLength--;
	}
	else if (_drawBarLength < _barLength)
	{
		_drawBarLength++;
	}
}

void HPUI::Draw()
{
	// プレイヤーがUIの近くにいるときは透明にする
	bool isPlayerNear = _player.GetDrawPos().x < kDrawPosX + (kGraphSizeW * kDrawScale) / 2 + kLowAlphaDis &&
						_player.GetDrawPos().y < kDrawPosY + (kGraphSizeH * kDrawScale) / 2 + kLowAlphaDis;
	// 敵がUIの近くにいるときは透明にする
	bool isEnemyNear = false;
	for (const auto& enemy : _pEnemies)
	{
		Vector2 pos = enemy->GetDrawPos();
		if (pos.x < kDrawPosX + (kGraphSizeW * kDrawScale) / 2 + kLowAlphaDis &&
			pos.y < kDrawPosY + (kGraphSizeH * kDrawScale) / 2 + kLowAlphaDis)
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

	DrawBox(kDrawBarL, kDrawBarT, kDrawBarR, kDrawBarB, 0x000000, true);	// HPないとこの黒
	DrawBox(kDrawBarL + _barLength, kDrawBarT, kDrawBarL + _drawBarLength, kDrawBarB, 0xff0000, true);	// HP減る量の赤
	DrawBox(kDrawBarL, kDrawBarT, kDrawBarL + _barLength, kDrawBarB, 0xffff00, true);	// HPあるとこの黄色
	// HPが増えているとき
	if (_barLength > _drawBarLength)
	{
		DrawBox(kDrawBarL + _drawBarLength, kDrawBarT, kDrawBarL + _barLength, kDrawBarB, 0x00ff00, true);	// HP増える量の緑
	}

	DrawRotaGraph(kDrawPosX, kDrawPosY, kDrawScale, 0.0, _handle, true);	// 枠
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}