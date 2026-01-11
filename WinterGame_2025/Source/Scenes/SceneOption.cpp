#include "SceneOption.h"
#include "../Game.h"
#include "Dxlib.h"
#include "../Systems/Input.h"
#include "../Systems/SoundManager.h"
#include "SceneManager.h"

namespace
{
	// ウィンドウサイズと位置
	constexpr int kWindowLeft = GlobalConstants::kScreenWidth / 4;
	constexpr int kWindowTop = GlobalConstants::kScreenHeight / 4;
	constexpr int kWindowWidth = GlobalConstants::kScreenWidth / 2;
	constexpr int kWindowHeight = GlobalConstants::kScreenHeight / 2;

	// 音量バーの幅と太さ
	constexpr int kSoundVolumeBarWidth = 500;
	constexpr int kSoundVolumeBarThickness = 3;
	// 現在の音量を表す円
	constexpr int kSoundVolumeCircleRadius = 10;
	constexpr int kSoundVolumeCircleOutlineRadius = 12;
}

SceneOption::SceneOption(SceneManager& manager):
	SceneBase(manager)
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	// 関数リストの初期化
	_drawFuncs.resize(static_cast<int>(OptionMenu::Num));
	_drawFuncs[static_cast<int>(OptionMenu::BgmVolume)] = [this](int nowIndex) {
		DrawBgmVolume(nowIndex);
		};
	_drawFuncs[static_cast<int>(OptionMenu::SeVolume)] = [this](int nowIndex) {
		DrawSeVolume(nowIndex);
		};
	_drawFuncs[static_cast<int>(OptionMenu::Back)] = [this](int nowIndex) {
		DrawBack(nowIndex);
		};

	// 音量を取得
	_bgmVolume = SoundManager::GetInstance().GetBGMVolume();
	_seVolume = SoundManager::GetInstance().GetSEVolume();
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
}

void SceneOption::Update(Input& input)
{
	// 上下ボタンで選択しているインデックスの変更
	if (input.IsTriggered("down"))
	{
		_selectIndex++;
		if (_selectIndex >= static_cast<int>(OptionMenu::Num))
		{
			_selectIndex = 0;
		}
	}
	if (input.IsTriggered("up"))
	{
		_selectIndex--;
		if (_selectIndex < 0)
		{
			_selectIndex = static_cast<int>(OptionMenu::Num) - 1;
		}
	}
	// 選択されているインデックスに応じた処理を実行
	switch (static_cast<OptionMenu>(_selectIndex))
	{
	case OptionMenu::BgmVolume:
		UpdateBgmVolume(input);
		break;
	case OptionMenu::SeVolume:
		UpdateSeVolume(input);
		break;
	case OptionMenu::Back:
		UpdateBack(input);
		break;
	}

	// 戻るボタンを押して戻る
	if (input.IsTriggered("back"))
	{
		_manager.PopScene();
	}
}

void SceneOption::Draw()
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	// 半透明黒背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(kWindowLeft, kWindowTop, kWindowLeft + kWindowWidth, kWindowTop + kWindowHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// それぞれの要素の描画
	for (int i = 0; i < static_cast<int>(OptionMenu::Num); i++)
	{
		_drawFuncs[i](_selectIndex);
	}

#ifdef _DEBUG
	DrawFormatString(kWindowLeft, kWindowTop, 0xffffff, "_selectIndex:%d", _selectIndex);
#endif
}

void SceneOption::UpdateBgmVolume(Input& input)
{
	// 左右入力で音量を調整
	if (input.IsPressed("left"))
	{
		_bgmVolume -= 2;
		if (_bgmVolume < 0)
		{
			_bgmVolume = 0;
		}
		SoundManager::GetInstance().ChangeVolume(SoundType::BGM, _bgmVolume);
	}
	if (input.IsPressed("right"))
	{
		_bgmVolume += 2;
		if (_bgmVolume > 255)
		{
			_bgmVolume = 255;
		}
		SoundManager::GetInstance().ChangeVolume(SoundType::BGM, _bgmVolume);
	}
}

void SceneOption::UpdateSeVolume(Input& input)
{
	// 左右入力で音量を調整
	if (input.IsPressed("left"))
	{
		_seVolume -= 2;
		if (_seVolume < 0)
		{
			_seVolume = 0;
		}
		SoundManager::GetInstance().ChangeVolume(SoundType::SE, _seVolume);
	}
	if (input.IsReleased("left"))
	{
		SoundManager::GetInstance().PlaySoundGame("Shot");
	}
	if (input.IsPressed("right"))
	{
		_seVolume += 2;
		if (_seVolume > 255)
		{
			_seVolume = 255;
		}
		SoundManager::GetInstance().ChangeVolume(SoundType::SE, _seVolume);
	}
	if (input.IsReleased("right"))
	{
		SoundManager::GetInstance().PlaySoundGame("Shot");
	}
}

void SceneOption::UpdateBack(Input& input)
{
	if (input.IsTriggered("decision"))
	{
		_manager.PopScene();
	}
}

void SceneOption::DrawBgmVolume(int nowIndex)
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	unsigned int color = 0xff0000;
	if (nowIndex == static_cast<int>(OptionMenu::BgmVolume)) color = 0x00ff00;

	// bgm音量バー
	DrawLine(screenW / 2 - kSoundVolumeBarWidth / 2, screenH / 2 - 50,
		screenW / 2 + kSoundVolumeBarWidth / 2, screenH / 2 - 50, 0xffffff, kSoundVolumeBarThickness);
	// bgm音量バーの現在値
	DrawCircle(static_cast<int>(screenW / 2 - kSoundVolumeBarWidth / 2 + (_bgmVolume / 255.0f) * kSoundVolumeBarWidth),
		static_cast<int>(screenH / 2 - 50), kSoundVolumeCircleOutlineRadius, 0xffffff, true);
	DrawCircle(static_cast<int>(screenW / 2 - kSoundVolumeBarWidth / 2 + (_bgmVolume / 255.0f) * kSoundVolumeBarWidth),
		static_cast<int>(screenH / 2 - 50), kSoundVolumeCircleRadius, color, true);
	// テキスト
	DrawString(screenW / 2 - kSoundVolumeBarWidth / 2, screenH / 2 - 82, "BGM", 0xffffff);
}

void SceneOption::DrawSeVolume(int nowIndex)
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	unsigned int color = 0xff0000;
	if (nowIndex == static_cast<int>(OptionMenu::SeVolume)) color = 0x00ff00;

	// se音量バー
	DrawLine(screenW / 2 - kSoundVolumeBarWidth / 2, screenH / 2 + 50,
		screenW / 2 + kSoundVolumeBarWidth / 2, screenH / 2 + 50, 0xffffff, kSoundVolumeBarThickness);
	// se音量バーの現在値
	DrawCircle(static_cast<int>(screenW / 2 - kSoundVolumeBarWidth / 2 + (_seVolume / 255.0f) * kSoundVolumeBarWidth),
		static_cast<int>(screenH / 2 + 50), kSoundVolumeCircleOutlineRadius, 0xffffff, true);
	DrawCircle(static_cast<int>(screenW / 2 - kSoundVolumeBarWidth / 2 + (_seVolume / 255.0f) * kSoundVolumeBarWidth),
		static_cast<int>(screenH / 2 + 50), kSoundVolumeCircleRadius, color, true);
	// テキスト
	DrawString(screenW / 2 - kSoundVolumeBarWidth / 2, screenH / 2 + 18, "SE", 0xffffff);
}

void SceneOption::DrawBack(int nowIndex)
{
	constexpr int screenW = GlobalConstants::kScreenWidth;
	constexpr int screenH = GlobalConstants::kScreenHeight;

	unsigned int color = 0xffffff;
	if (nowIndex == static_cast<int>(OptionMenu::Back)) color = 0x00ff00;

	DrawString(screenW / 2, screenH / 2 + 150, "戻る", color);
}
