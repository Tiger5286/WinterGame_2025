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
	int _bgHandle = -1;
	int _gameOverHandle = -1;

	int _score;
	bool _isSelectRestart;

	Stages _playedStage;
};

