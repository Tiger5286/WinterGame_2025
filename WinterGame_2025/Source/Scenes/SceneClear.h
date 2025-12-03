#pragma once
#include "SceneBase.h"
class SceneClear : public SceneBase
{
public:
	SceneClear(SceneManager& manager,int score);
	~SceneClear();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _bgHandle = -1;
	int _clearLogoHandle = -1;

	const int _score;
	float _dispScore = 0.0f;
};

