#pragma once
#include "SceneBase.h"
#include <vector>
#include <functional>
#include <string>
#include <map>

class SceneStageSelect : public SceneBase
{
public:
	SceneStageSelect(SceneManager& manager);
	~SceneStageSelect();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _selectIndex;	// 現在選択中のインデックス
	std::vector<std::string> _stageList;	// ステージリスト
	std::map<std::string, std::function<void()>> _execTable;	// ステージ名と実行関数の対応表
};