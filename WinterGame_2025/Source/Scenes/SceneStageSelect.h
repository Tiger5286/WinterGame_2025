#pragma once
#include "SceneBase.h"
#include "../Game.h"
#include <vector>
#include <functional>
#include <string>
#include <map>

class SceneStageSelect : public SceneBase
{
public:
	SceneStageSelect(SceneManager& manager,Stages playedStage = Stages::Tutorial);
	~SceneStageSelect();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _bgHandle = -1;
	int _stageUIHandle = -1;

	int _frame;
	// UIがどっちに移動しているかどうかのフラグ
	// (UIが右に移動=左入力)
	bool _isUIMoveRight = true;
	int _bgOffsetX = 0;
	// 隠しステージ開放に使う右入力カウント
	int _inputRightCount = 0;

	int _selectIndex;	// 現在選択中のインデックス
	std::vector<std::string> _stageList;	// ステージリスト
	std::map<std::string, std::function<void()>> _execTable;	// ステージ名と実行関数の対応表
};