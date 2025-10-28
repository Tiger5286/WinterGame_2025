#pragma once
#include "GameObject.h"
#include "Input.h"
#include <memory>
#include <vector>

class Bullet;

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 必要な情報を受け取る関数
	/// </summary>
	/// <param name="input">Inputクラスのインスタンス</param>
	/// <param name="pBullets">Bulletの配列情報</param>
	void SetContext(const Input& input,std::vector<std::shared_ptr<Bullet>>& pBullets);

private:
	void Jump();
	void Move();
	void Shot();

private:
	Input _input;
	std::vector<std::shared_ptr<Bullet>> _pBullets;
	int _jumpFrame;
	bool _isGround;
	bool _isJumping;
	bool _isTurn;	// true:左向き/false:右向き
	Vector2 _shotPos;
};