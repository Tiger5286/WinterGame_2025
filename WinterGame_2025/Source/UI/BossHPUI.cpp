#include "BossHPUI.h"
#include <cmath>
#include "DxLib.h"
#include "../GameObjects/Enemies/Enemy.h"
#include "../GameObjects/Player.h"
#include "../Systems/EnemyManager.h"

namespace
{
	// UI画像の情報
	constexpr int kGraphSizeW = 2400;
	constexpr int kGraphSizeH = 500;
	// バーの情報
	constexpr int kBarL = 136;
	constexpr int kBarT = 189;
	constexpr int kBarR = 2094;
	constexpr int kBarB = 314;
	// UIの描画情報
	constexpr float kDrawScale = 0.4f;
	constexpr int kDrawPosX = 1400;
	constexpr int kDrawPosY = 130;
	constexpr int kDrawPosL = static_cast<int>(kDrawPosX - (kGraphSizeW * kDrawScale) / 2);
	constexpr int kDrawPosT = static_cast<int>(kDrawPosY - (kGraphSizeH * kDrawScale) / 2);
	// バーの描画情報
	constexpr int kDrawBarL = static_cast<int>(kDrawPosL + (kBarL * kDrawScale));
	constexpr int kDrawBarR = static_cast<int>(kDrawPosL + (kBarR * kDrawScale));
	constexpr int kDrawBarT = static_cast<int>(kDrawPosT + (kBarT * kDrawScale));
	constexpr int kDrawBarB = static_cast<int>(kDrawPosT + (kBarB * kDrawScale)) + 1;	// 隙間を埋めるために1足す
	constexpr int kBarLength = kDrawBarR - kDrawBarL;

	// 透明になる領域を余分に確保する量
	constexpr int kLowAlphaDis = 100;
}

BossHPUI::BossHPUI(int handle, Player& player, const std::shared_ptr<EnemyManager> pEnemyManager) :
	_handle(handle),
	_bossMaxHp(pEnemyManager->GetTotalBossHp()),
	_barLength(0),
	_drawBarLength(0),
	_alpha(255),
	_player(player),
	_pEnemyManager(pEnemyManager)
{
}

BossHPUI::~BossHPUI()
{
}

void BossHPUI::Init()
{
}

void BossHPUI::Update()
{
	// ボスのhpからバーの長さを出す
	_barLength = static_cast<int>(static_cast<float>(_pEnemyManager->GetTotalBossHp()) / static_cast<float>(_bossMaxHp) * kBarLength);
	// lerpでいい感じに減らす
	_drawBarLength = static_cast<int>(std::lerp(_drawBarLength, _barLength, 0.05f));
}

void BossHPUI::Draw()
{
	// プレイヤーがUIの近くにいるときは透明にする
	bool isPlayerNear = _player.GetDrawPos().y < kDrawBarB + kLowAlphaDis &&
						_player.GetDrawPos().x > kDrawBarL - kLowAlphaDis;
	// 敵がUIの近くにいるときは透明にする
	bool isEnemyNear = false;
	for (const auto& enemy : _pEnemyManager->GetEnemies())
	{
		Vector2 enemyPos = enemy->GetDrawPos();
		if (enemyPos.y < kDrawBarB + kLowAlphaDis &&
			enemyPos.x > kDrawBarL - kLowAlphaDis)
		{
			isEnemyNear = true;
			break;
		}
	}
	// 透明度をlerpでいい感じに変える
	if (isPlayerNear || isEnemyNear)
	{
		_alpha = static_cast<int>(std::lerp(_alpha, 64, 0.2f));
	}
	else
	{
		_alpha = static_cast<int>(std::lerp(_alpha, 255, 0.2f));
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
	DrawBox(kDrawBarL + _drawBarLength, kDrawBarT, kDrawBarR, kDrawBarB, 0x000000, true);	// HPないとこの黒
	DrawBox(kDrawBarL + _barLength, kDrawBarT, kDrawBarL + _drawBarLength, kDrawBarB, 0xff0000, true);	// HP減る量の赤
	DrawBox(kDrawBarL, kDrawBarT, kDrawBarL + _barLength, kDrawBarB, 0xffff00, true);	// HPあるとこの黄色
	DrawRotaGraph(kDrawPosX, kDrawPosY, kDrawScale, 0.0, _handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}