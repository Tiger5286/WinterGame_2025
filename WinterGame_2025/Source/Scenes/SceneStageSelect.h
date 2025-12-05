#pragma once
#include "SceneBase.h"
#include <vector>
#include <functional>
#include <string>
#include <map>

class SceneStageSelect : public SceneBase
{
public:
	SceneStageSelect(SceneManager& manager,int selectIndex = 0);
	~SceneStageSelect();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _bgHandle = -1;
	int _stageUIHandle = -1;

	int _frame;
	bool _isUIMoveRight = true;

	int _selectIndex;	// 現在選択中のインデックス
	std::vector<std::string> _stageList;	// ステージリスト
	std::map<std::string, std::function<void()>> _execTable;	// ステージ名と実行関数の対応表
};