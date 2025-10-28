#include "SceneMain.h"
#include "DxLib.h"
#include "Application.h"
#include "input.h"
#include "Player.h"
#include <cassert>

SceneMain::SceneMain() :
	_frameCount(0)
{
	_playerH = LoadGraph("data/Player/Player.png");
	assert(_playerH != -1);
	_playerShotH = LoadGraph("data/Player/Shot.png");
	assert(_playerShotH != -1);

	_pPlayer = std::make_shared<Player>();
	_pPlayer->SetHandle(_playerH);
}

SceneMain::~SceneMain()
{
	DeleteGraph(_playerH);
	DeleteGraph(_playerShotH);
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