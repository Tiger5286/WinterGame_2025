#pragma once
#include "SceneBase.h"
#include <array>
class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	~SceneTitle();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _frame = 0;
	// 画像ハンドル
	int _bgHandle = -1;
	int _titleHandle = -1;

	// メニューボタンハンドル(スタート、オプション、終了)
	std::array<int, 3> _menuHandles = { -1,-1,-1 };

	// フォントハンドル
	int _fontHandle = -1;

	// 選択中のメニューインデックス
	int _selectIndex = 0;
};

