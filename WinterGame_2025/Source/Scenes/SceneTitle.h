#pragma once
#include "SceneBase.h"
#include <array>
#include <memory>

class TitlePlayer;
class TitleEnemy;
class TitleBullet;
class EffectManager;

class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	~SceneTitle();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	/// <summary>
	/// 
	/// </summary>
	void OnTriggeredStart();

private:
	int _frame = 0;

	// 画像ハンドル
	int _bgHandle = -1;
	int _titleHandle = -1;
	int _mapHandle = -1;

	// メニューボタンハンドル(スタート、オプション、終了)
	std::array<int, 3> _menuHandles = { -1,-1,-1 };

	// フォントハンドル
	int _fontHandle = -1;

	// 選択中のメニューインデックス
	int _selectIndex = 0;

	// スタートボタンが押されたかどうか
	bool _isTriggeredStart = false;
	// フレームカウント
	int _frameCount = 0;

	std::shared_ptr<TitlePlayer> _pPlayer = nullptr;
	std::shared_ptr<TitleEnemy> _pEnemy = nullptr;
	std::array<std::shared_ptr<TitleBullet>, 10> _pBullets = { nullptr };
	
	std::shared_ptr<EffectManager> _pEffectManager = nullptr;
};

