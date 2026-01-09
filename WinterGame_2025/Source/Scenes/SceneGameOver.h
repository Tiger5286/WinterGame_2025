#pragma once
#include "SceneBase.h"
#include "SceneMain.h"

class SceneManager;

class SceneGameOver : public SceneBase
{
public:
	SceneGameOver(SceneManager& manager,Stages playedStage,int score);
	~SceneGameOver();

	void Init();
	void Update(Input& input);
	void Draw();
private:
	/*画像ハンドル*/
	int _bgHandle = -1;	// 背景
	int _gameOverHandle = -1;	// ロゴ
	
	int _restartTextHandle = -1;	// 再挑戦テキストハンドル
	int _backTextHandle = -1;	// 戻るテキストハンドル

	int _score;	// スコア保存用
	bool _isSelectRestart;	// どっちを選択しているかフラグ

	// 汎用フレームカウント
	int _frame = 0;

	Stages _playedStage;	// ステージ保存用
};

