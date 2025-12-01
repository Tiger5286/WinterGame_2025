#pragma once
#include "GameObject.h"
#include "../Systems/Input.h"
#include <memory>
#include <vector>

class Bullet;
class Map;

class Player : public GameObject
{
public:
	Player(int playerH,int playerWhiteH, int chargeParticleH,int shotH,int chargeShotH);
	~Player();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;

	/// <summary>
	/// 必要な情報を受け取る関数
	/// </summary>
	/// <param name="input">Inputクラスのインスタンス</param>
	/// <param name="pBullets">Bulletの配列情報</param>
	void SetContext(const Input& input,std::vector<std::shared_ptr<Bullet>>& pBullets);

	void InitPosFromStage(const std::vector<uint16_t>& objectData,const Size mapSize);

	void TakeDamage();	// ダメージを受ける
	bool GetIsAlive() { return _isAlive; }
	int GetHp() { return _hp; }
	int GetMaxHp();

private:
	void Jump();
	void Move();
	void Slide();
	void MoveSpeedLimit();	// 移動速度制限
	void MoveResistance();	// 移動抵抗
	void MoveAreaLimit(Map& map);	// 移動範囲制限
	void DamageUpdate();	// 被ダメージ時の無敵時間などの処理
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
		int whiteHandle = -1;
		bool isTurn = false;
		void Draw(Vector2 offset);
	};

private:
	Input _input;
	std::vector<std::shared_ptr<Bullet>> _pBullets;

	// 画像ハンドル
	int _playerH;
	int _playerWhiteH;
	int _chargeParticleH;	// チャージエフェクトの画像ハンドル
	int _shotH;
	int _chargeShotH;

	// アニメーション
	Animation _idleAnim;
	Animation _moveAnim;
	Animation _damageAnim;
	Animation _jumpAnim;
	Animation _fallAnim;
	Animation _dashAnim;
	Animation _slideAnim;
	Animation _deathAnim;

	Animation _shotFlashAnim;
	Animation _chargeShotFlashAnim;
	Animation _ChargeParticleAnim;

	int _hp;
	bool _isAlive;

	// ジャンプ関連変数
	int _jumpFrame;		// ジャンプ長押し時間
	bool _isJumping;	// ジャンプ中フラグ

	bool _isTurn;	// true:左向き/false:右向き
	Vector2 _shotPos;	// 弾を召喚する位置

	bool _isSlide;	// 壁スライド状態かどうか

	// ダメージ関連変数
	bool _isCanControll; // プレイヤー操作可能フラグ
	int _invincibleFrame; // 無敵時間
	bool _isFrickering; // 点滅中フラグ

	// ダッシュ関連変数
	int _dashCoolTime;	// ダッシュのクールタイム
	int _dashFrame;		// ダッシュ中の時間
	bool _isDashing;	// ダッシュ中フラグ
	bool _isTurnDashing;// ダッシュ中の向き // true:左向き/false:右向き
	std::vector<PlayerAfterimage> _playerAfterimage;	// 残像を格納する配列

	// チャージショット関連変数
	int _chargeFrame;

#ifdef _DEBUG
	// デバッグ用
	bool _isCanFly;
#endif
};