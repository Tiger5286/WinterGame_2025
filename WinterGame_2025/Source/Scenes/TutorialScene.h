#pragma once
#include "SceneBase.h"
#include "SceneMain.h"	// TutorialStepを使うためにインクルード
#include <string>

class TutorialScene :
    public SceneBase
{
public:
	TutorialScene(SceneManager& manager,TutorialStep nowStep);
	~TutorialScene();

	void Init() override;
	void Update(Input& input) override;
	void Draw() override;
private:
	int _fontHandle = -1;
	int _buttonHandle = -1;
	int _buttonOutlineHandle = -1;

	std::string _tutorialText;
	int _frame = 0;
};

