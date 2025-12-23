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
	std::string _tutorialText;
};

