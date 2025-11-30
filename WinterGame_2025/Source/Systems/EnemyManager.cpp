#include "EnemyManager.h"
#include "../Game.h"
#include "Dxlib.h"
#include <cassert>

#include "../GameObjects/Enemies/WalkEnemy.h"
#include "../GameObjects/Enemies/FlyEnemy.h"
#include "../GameObjects/Enemies/JumpEnemy.h"

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer):
	_pPlayer(pPlayer)
{
	_walkEnemyH = LoadGraph("data/Enemys/WalkEnemy.png");
	assert(_walkEnemyH != -1);
	_flyEnemyH = LoadGraph("data/Enemys/FlyEnemy.png");
	assert(_flyEnemyH != -1);
	_jumpEnemyH = LoadGraph("data/Enemys/JumpEnemy.png");
	assert(_jumpEnemyH != -1);

}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update()
{
}

void EnemyManager::Draw()
{
}

void EnemyManager::Spawn(const std::vector<uint16_t>& objectData, Size size)
{ 
	for (int w = 0; w < size.w; w++)
	{
		for (int h = 0; h < size.h; h++)
		{
			auto index = w + h * size.w;
			auto data = static_cast<ObjectData>(objectData[index]);
			switch (data)
			{
			case ObjectData::WalkEnemyIdle:
				_enemies.push_back(std::make_shared<WalkEnemy>(Vector2(static_cast<float>(w), static_cast<float>(h)), _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
				break;
			}
		}
	}
}
