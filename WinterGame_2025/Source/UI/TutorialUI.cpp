#include "TutorialUI.h"
#include "Dxlib.h"
#include "../GameObjects/Player.h"
#include <string>
#include <cassert>

namespace
{
	// 画像リスト
	enum class TutorialGraphs
	{
		ButtonA,
		ButtonAOutline,
		ButtonB,
		ButtonBOutline,
		ButtonX,
		ButtonXOutline,
		DPad,
		DPadLeft,
		DPadRight,

		Num
	};
	// ロードする画像のファイル名
	const std::string kGraphFileNames[] =
	{
		"data/UI/Buttons/button_a.png",
		"data/UI/Buttons/button_a_outline.png",
		"data/UI/Buttons/button_b.png",
		"data/UI/Buttons/button_b_outline.png",
		"data/UI/Buttons/button_x.png",
		"data/UI/Buttons/button_x_outline.png",
		"data/UI/Buttons/dpad.png",
		"data/UI/Buttons/dpad_left.png",
		"data/UI/Buttons/dpad_right.png"
	};
	// 画像の枚数とenumの数が一致しているか確認
	constexpr int size = sizeof(kGraphFileNames) / sizeof(kGraphFileNames[0]);
	static_assert(static_cast<int>(TutorialGraphs::Num) == size);

	/*定数*/
	constexpr int kDrawOffsetY = 100;
	constexpr int kDrawOffsetX = 30;	// ボタンUIを2つ表示するときに横軸ずらす用
	// プレイヤーのステータス関連
	constexpr int kChargeFrameMax = 90;
	constexpr int kDashCoolTime = 120;
	// ゲージ関連定数
	constexpr int kGaugeLength = 80;
	constexpr int kGaugeHeight = 10;
	constexpr int kGaugeBgMargin = 2;
}

TutorialUI::TutorialUI(Player& player):
	_player(player)
{
	for (int i = 0; i < static_cast<int>(TutorialGraphs::Num); i++)
	{
		_graphHandles.push_back(LoadGraph(kGraphFileNames[i].c_str()));
		assert(_graphHandles.back() != -1 && "チュートリアル画像を正しくロードできませんでした");
	}
}

TutorialUI::~TutorialUI()
{
	for (const auto& handle : _graphHandles)
	{
		DeleteGraph(handle);
	}
}

void TutorialUI::Update(Input& input)
{
	_frame++;

	const auto& playerPos = _player.GetPos();
	switch (_step)
	{
	case TutorialUISteps::Move:
		if (playerPos.x > 1300)
		{
			_step = TutorialUISteps::Shot;
			_draw = &TutorialUI::DrawShot;
		}
		break;
	case TutorialUISteps::Shot:
		if (playerPos.x > 1800)
		{
			_step = TutorialUISteps::Jump;
			_draw = &TutorialUI::DrawJump;
		}
		break;
	case TutorialUISteps::Jump:
		if (playerPos.x > 2600)
		{
			_step = TutorialUISteps::ChargeShot;
			_draw = &TutorialUI::DrawChargeShot;
		}
		break;
	case TutorialUISteps::ChargeShot:
		if (playerPos.x > 4100)
		{
			_step = TutorialUISteps::Dash;
			_draw = &TutorialUI::DrawDash;
		}
		break;
	case TutorialUISteps::Dash:
		if (playerPos.x > 5900)
		{
			_step = TutorialUISteps::WallJump;
			_draw = &TutorialUI::DrawWallJump;
		}
		break;
	case TutorialUISteps::WallJump:
		if (playerPos.x > 6300)
		{
			_step = TutorialUISteps::None;
			_draw = nullptr;
		}
		break;
	}
}

void TutorialUI::Draw(const Vector2& offset)
{
	Vector2 drawPos = _player.GetDrawPos();
	drawPos.y -= kDrawOffsetY;
	
	if (_draw != nullptr)
	{
		(this->*_draw)(drawPos);
	}
}

void TutorialUI::DrawChargeGauge(const Vector2& drawPos)
{
	// チャージショットのゲージの左端座標
	const float drawLeftPos = drawPos.x - kGaugeLength / 2;

	// チャージゲージの背景の黒箱描画
	DrawBox(drawLeftPos - kGaugeBgMargin,
		drawPos.y - kGaugeHeight / 2 - kGaugeBgMargin,
		drawLeftPos + kGaugeLength + kGaugeBgMargin,
		drawPos.y + kGaugeHeight / 2 + kGaugeBgMargin,
		0x000000, true);
	// チャージ状況を取得しゲージの長さを計算
	int nowGaugeLength = static_cast<int>(_player.GetChargeFrame() / static_cast<float>(kChargeFrameMax) * kGaugeLength);
	// ゲージの長さの最大を越さないようにする
	if (nowGaugeLength > kGaugeLength)
	{
		nowGaugeLength = kGaugeLength;
	}
	// ゲージ本体を描画
	unsigned int color = 0x00ffff;
	// 最大までたまっていたら点滅させる
	if (_player.GetChargeFrame() > kChargeFrameMax && _frame % 10 < 5)
	{
		color = 0xffffff;
	}
	DrawBox(drawLeftPos,
		drawPos.y - kGaugeHeight / 2,
		drawLeftPos + nowGaugeLength,
		drawPos.y + kGaugeHeight / 2,
		color, true);
}

void TutorialUI::DrawDashGauge(const Vector2& drawPos)
{
	// ゲージの左端座標
	const float drawLeftPos = drawPos.x - kGaugeLength / 2;

	// ゲージの背景の黒箱描画
	DrawBox(drawLeftPos - kGaugeBgMargin,
		drawPos.y - kGaugeHeight / 2 - kGaugeBgMargin,
		drawLeftPos + kGaugeLength + kGaugeBgMargin,
		drawPos.y + kGaugeHeight / 2 + kGaugeBgMargin,
		0x000000, true);
	// チャージ状況を取得しゲージの長さを計算
	int nowGaugeLength = static_cast<int>((kDashCoolTime - _player.GetDashCoolTime()) / static_cast<float>(kDashCoolTime) * kGaugeLength);
	DrawFormatString(drawPos.x, drawPos.y - 100, 0xffffff, "%d", _player.GetDashCoolTime());
	// ゲージの長さの最大を越さないようにする
	if (nowGaugeLength > kGaugeLength)
	{
		nowGaugeLength = kGaugeLength;
	}
	// ゲージ本体を描画
	unsigned int color = 0xff0000;
	// 最大までたまっていたら色を変える
	if (_player.GetDashCoolTime() <= 0)
	{
		color = 0x00ffff;
	}
	DrawBox(drawLeftPos,
		drawPos.y - kGaugeHeight / 2,
		drawLeftPos + nowGaugeLength,
		drawPos.y + kGaugeHeight / 2,
		color, true);
}

void TutorialUI::DrawMove(const Vector2& drawPos)
{
	int handle = -1;
	if (_frame % 40 < 20)
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::DPadRight)];
	}
	else
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::DPadLeft)];
	}
	DrawRotaGraph(drawPos.x, drawPos.y, 1.0, 0.0, handle, true);
}

void TutorialUI::DrawShot(const Vector2& drawPos)
{
	int handle = -1;
	if (_frame % 30 < 10)
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonXOutline)];
	}
	else
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonX)];
	}
	DrawRotaGraph(drawPos.x, drawPos.y, 1.0, 0.0, handle, true);
}

void TutorialUI::DrawJump(const Vector2& drawPos)
{
	int handle = -1;
	if (_frame % 40 < 20)
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonA)];
	}
	else
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonAOutline)];
	}
	DrawRotaGraph(drawPos.x, drawPos.y, 1.0, 0.0, handle, true);
}

void TutorialUI::DrawChargeShot(const Vector2& drawPos)
{
	int handle = -1;
	if (_frame % 120 < 20)
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonX)];
	}
	else
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonXOutline)];
	}
	DrawRotaGraph(drawPos.x, drawPos.y, 1.0, 0.0, handle, true);

	Vector2 gaugeDrawPos = drawPos;
	gaugeDrawPos.y += 35;
	DrawChargeGauge(gaugeDrawPos);
}

void TutorialUI::DrawDash(const Vector2& drawPos)
{
	int handle = -1;
	if (_frame % 40 < 10)
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonBOutline)];
	}
	else
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonB)];
	}
	DrawRotaGraph(drawPos.x, drawPos.y, 1.0, 0.0, handle, true);

	Vector2 gaugeDrawPos = drawPos;
	gaugeDrawPos.y += 35;
	DrawDashGauge(gaugeDrawPos);
}

void TutorialUI::DrawWallJump(const Vector2& drawPos)
{
	int handle = -1;
	if (_frame % 30 < 10)
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonAOutline)];
	}
	else
	{
		handle = _graphHandles[static_cast<int>(TutorialGraphs::ButtonA)];
	}
	DrawRotaGraph(drawPos.x + kDrawOffsetX, drawPos.y, 1.0, 0.0, handle, true);
	DrawRotaGraph(drawPos.x - kDrawOffsetX, drawPos.y, 1.0, 0.0, _graphHandles[static_cast<int>(TutorialGraphs::DPadRight)], true);
}
