#pragma once
#include "SceneBase.h"
#include <vector>
#include <string>
#include <map>
#include <functional>

enum class OptionMenu
{
	BgmVolume,
	SeVolume,
	Back,

	Num
};

class SceneOption :
    public SceneBase
{
public:
	SceneOption(SceneManager& manager);
	~SceneOption();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	void UpdateBgmVolume(Input& input);
	void UpdateSeVolume(Input& input);
	void UpdateBack(Input& input);
	void DrawBgmVolume(int nowIndex);
	void DrawSeVolume(int nowIndex);
	void DrawBack(int nowIndex);

private:
	int _fontHandle = -1;

	int _selectIndex = 0;
	std::vector<std::function<void(int nowIndex)>> _drawFuncs;

	int _bgmVolume;
	int _seVolume;
};

