#pragma once
#include "SceneBase.h"
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

private:
	int _selectIndex = 0;

	int _bgmVolume;
	int _seVolume;
};

