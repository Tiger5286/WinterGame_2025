#pragma once
#include "SceneBase.h"
#include "SceneMain.h"

class SceneManager;

class SceneGameOver : public SceneBase
{
public:
	SceneGameOver(SceneManager& manager,Stages playedStage);
	~SceneGameOver();

	void Init();
	void Update(Input& input);
	void Draw();
private:

	bool _isSelectRestart;

	Stages _playedStage;
};

