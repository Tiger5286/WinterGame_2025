#pragma once
#include "SceneBase.h"
#include "../Game.h"

class SceneClear : public SceneBase
{
public:
	SceneClear(SceneManager& manager,int score,Stages clearStage);
	~SceneClear();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	/*画像ハンドル*/
	int _bgHandle = -1;	// 背景
	int _clearLogoHandle = -1;	// クリアUI

	int _scoreTextHandle = -1;	// スコアテキスト画像
	int _numberTextHandle = -1;	// 数字テキスト画像

	int _updateHighScoreHandle = -1;	// ハイスコア更新UI
	int _backTextHandle = -1;	// 戻るUI

	// 汎用フレームカウント
	int _frame = 0;

	// スコア
	const int _score;
	// 表示用スコア
	float _dispScore = 0.0f;
	// ハイスコアを更新したかどうか
	bool _isUpdateScore = false;
	// クリアしたステージ
	Stages _clearStage;
};

