#pragma once
#include "SceneBase.h"
#include <vector>
#include <string>
#include <functional>
#include <map>

class DebugScene :
    public SceneBase
{
public:
	DebugScene(SceneManager& manager);
	~DebugScene();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _selectIndex;	// 現在選択中のインデックス
	std::vector<std::string> _menuList;	// メニューリスト
	std::map<std::string, std::function<void()>> _execTable;	// メニュー名と実行関数の対応表
};

