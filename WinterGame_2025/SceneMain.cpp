#include "SceneMain.h"
#include "DxLib.h"
#include "Application.h"
#include "input.h"
#include "Player.h"

SceneMain::SceneMain() :
	_frameCount(0)
{
	_pPlayer = std::make_shared<Player>();
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	_pPlayer->Init();
}

void SceneMain::Update(Input input)
{
	_frameCount++;
	_pPlayer->SetInput(input);
	_pPlayer->Update();
}

void SceneMain::Draw()
{
	_pPlayer->Draw();

	DrawString(0,0,"SceneMain",0xffffff);
	DrawFormatString(0, 16, 0xffffff, "FRAME:%d", _frameCount);
}