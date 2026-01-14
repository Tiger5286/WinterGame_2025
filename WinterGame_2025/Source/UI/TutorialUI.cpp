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

}

void TutorialUI::Draw(const Vector2& offset)
{
	Vector2 drawPos;
	drawPos.x = _player._pos.x - offset.x;
	drawPos.y = _player._pos.y - 100 - offset.y;
	DrawString(drawPos.x, drawPos.y, "Use Arrow Keys to Move", GetColor(255, 255, 255));
}