#pragma once
#include "GameObject.h"
#include "Input.h"
#include <memory>

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 入力を受け取る関数(Updateで毎フレーム呼び出す)
	/// </summary>
	/// <param name="input">Inputクラスのインスタンス</param>
	void SetInput(const Input& input);
private:
	Input _input;
	int _jumpFrame;
	bool _isGround;
	bool _isJumping;
	bool _isTurn;	// true:左向き/false:右向き
};

