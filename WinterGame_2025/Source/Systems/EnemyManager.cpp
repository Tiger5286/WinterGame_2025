#include "EnemyManager.h"
#include "../Game.h"
#include "Dxlib.h"
#include <cassert>

#include "../Systems/Camera.h"

#include "../GameObjects/Enemies/WalkEnemy.h"
#include "../GameObjects/Enemies/FlyEnemy.h"
#include "../GameObjects/Enemies/JumpEnemy.h"

EnemyManager::EnemyManager(std::shared_ptr<Player> pPlayer, std::shared_ptr<Map> pMap, std::shared_ptr<Camera> pCamera):
	_pPlayer(pPlayer),
	_pMap(pMap),
	_pCamera(pCamera)
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
	DeleteGraph(_walkEnemyH);
	DeleteGraph(_flyEnemyH);
	DeleteGraph(_jumpEnemyH);
}

void EnemyManager::Update()
{
	for (auto& enemy : _pEnemies)
	{
		if (enemy != nullptr)
		{
			float toCameraDisX = enemy->GetPos().x - _pCamera->GetPos().x;	// “G‚ÆƒJƒƒ‰‚Ì‹——£
			// ƒJƒƒ‰‚Ì‰æ–Ê“à‚É‚¢‚é“G‚¾‚¯XV‚·‚é
			if (abs(toCameraDisX) < GlobalConstants::kScreenWidth / 2 + 100)
			{
				enemy->Update(*_pMap);
			}
			// ‘Ì—Í‚ª0ˆÈ‰º‚É‚È‚Á‚½‚çÁ‚·
			if (enemy->GetHp() <= 0)
			{
				enemy->Delete();
				// €‚ñ‚¾“G‚ğvector‚©‚çíœ‚·‚é(‚ ‚ñ‚ÜˆÓ–¡‚í‚©‚ç‚ñ)
				_pEnemies.erase(
					std::remove_if(_pEnemies.begin(), _pEnemies.end(),
						[](const std::shared_ptr<Enemy>& enemy) {
							return !enemy->GetIsAlive();
						}),
					_pEnemies.end()
				);
			}
		}
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : _pEnemies)
	{
		if (enemy != nullptr)
		{
			enemy->Draw(_pCamera->GetDrawOffset());
		}
	}
}

void EnemyManager::LoadEnemies(const std::vector<uint16_t>& objectData, Size size)
{ 
	for (int w = 0; w < size.w; w++)
	{
		for (int h = 0; h < size.h; h++)
		{
			auto index = w + h * size.w;
			auto data = static_cast<ObjectData>(objectData[index]);
			Vector2 pos = Vector2(static_cast<float>(w), static_cast<float>(h));
			switch (data)
			{
			case ObjectData::WalkEnemyIdle:
				_pEnemies.push_back(std::make_shared<WalkEnemy>(pos, _pPlayer, _walkEnemyH, WalkEnemyState::Idle, false));
				break;
			case ObjectData::WalkEnemyMoveLeft:
				_pEnemies.push_back(std::make_shared<WalkEnemy>(pos, _pPlayer, _walkEnemyH, WalkEnemyState::Move, true));
				break;
			case ObjectData::WalkEnemyMoveRight:
				_pEnemies.push_back(std::make_shared<WalkEnemy>(pos, _pPlayer, _walkEnemyH, WalkEnemyState::Move, false));
				break;
			case ObjectData::FlyEnemyIdle:
				_pEnemies.push_back(std::make_shared<FlyEnemy>(pos, _pPlayer, _flyEnemyH, FlyEnemyState::Idle));
				break;
			case ObjectData::FlyEnemyMove:
				_pEnemies.push_back(std::make_shared<FlyEnemy>(pos, _pPlayer, _flyEnemyH, FlyEnemyState::Move));
				break;
			case ObjectData::JumpEnemy:
				_pEnemies.push_back(std::make_shared<JumpEnemy>(pos, _pPlayer, _jumpEnemyH));
				break;
			default:
				break;
			}
		}
	}
}
