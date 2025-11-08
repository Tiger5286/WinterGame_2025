#pragma once
#include "GameObject.h"
#include "Input.h"
#include <memory>
#include <vector>

class Bullet;
class Map;

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Update(Map& map);
	void Draw(Vector2 offset) override;

	void SetHandle(int playerH, int chargeParticleH);

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
	void ChargeShot();
	void Dash();
	void UpdateAnim();
	void UpdateAfterimage();

	// 残像を表す構造体
	struct PlayerAfterimage
	{
		Vector2 pos = { 0.0f,0.0f };
		int frame = 0;
		int handle = -1;
		bool isTurn = false;
		void Draw(Vector2 offset);
	};

private:
	Input _input;
	std::vector<std::shared_ptr<Bullet>> _pBullets;

	// 画像ハンドル
	int _playerH;
	int _chargeParticleH;	// チャージエフェクトの画像ハンドル

	// アニメーション
	Animation _idleAnim;
	Animation _moveAnim;
	Animation _jumpAnim;
	Animation _fallAnim;
	Animation _dashAnim;

	Animation _ChargeParticleAnim;

	// ジャンプ関連変数
	int _jumpFrame;		// ジャンプ長押し時間
	bool _isJumping;	// ジャンプ中フラグ

	bool _isTurn;	// true:左向き/false:右向き
	Vector2 _shotPos;	// 弾を召喚する位置

	// ダッシュ関連変数
	int _dashCoolTime;	// ダッシュのクールタイム
	int _dashFrame;		// ダッシュ中の時間
	bool _isDashing;	// ダッシュ中フラグ
	bool _isTurnDashing;// ダッシュ中の向き // true:左向き/false:右向き
	std::vector<PlayerAfterimage> _playerAfterimage;	// 残像を格納する配列

	// チャージショット関連変数
	int _chargeFrame;
	bool _isCharging;
};