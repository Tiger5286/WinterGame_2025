#pragma once
#include "SceneBase.h"
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "../Game.h"

class ScenePause :
    public SceneBase
{
public:
    ScenePause(SceneManager& manager,Stages playingStage);
    ~ScenePause();

	void Init() override;
    void Update(Input& input) override;
    void Draw() override;

private:
	Stages _playingStage;    // ポーズ画面に入る前のステージ情報
    int _selectIndex = 0;
	std::vector<std::string> _menuList;     // ポーズ画面で選択できるメニューリスト
	std::map<std::string, std::function<void()>> _execTable;    // メニューリストに対応する実行関数テーブル
};