#pragma once
#include "GameObject.h"
#include "../Systems/Input.h"
#include <memory>
#include <vector>

class Bullet;
class Map;
class BulletManager;
class EffectManager;
class TutorialUI;

class Player : public GameObject
{
	friend class TutorialUI;
public:
	Player(BulletManager& bulletManager,EffectManager& effectManager, bool isTutorial,int firstHp = 5);
	~Player();

	void Init() override;
	void Update(Map& map) override;
	void Draw(Vector2 offset) override;

	/// <summary>
	/// 必要な情報を受け取る関数
	/// </summary>
	/// <param name="input">Inputクラスのインスタンス</param>
	void SetContext(const Input& input);

	void InitPosFromStage(const std::vector<uint16_t>& objectData,const Size mapSize);

	void TakeDamage();	// ダメージを受ける
	bool GetIsAlive() const { return _isAlive; }
	int GetHp() const { return _hp; }
	int GetMaxHp() const;
	const int& GetHpRef() const { return _hp; }
	Vector2 GetDrawPos() const;
	/// <summary>
	/// プレイヤーを回復させる
	/// </summary>
	/// <param name="healAmount">回復量</param>
	void Heal(int healAmount);

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
	void DrawEffect();

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
	BulletManager& _bulletManager;
	EffectManager& _effectManager;
	std::shared_ptr<TutorialUI> _pTutorialUI = nullptr;

	// 画像ハンドル
	std::vector<int> _graphHandles;

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
	Animation _chargeParticleAnim;

	int _frame = 0;
	int _hp;
	bool _isAlive;
	Vector2 _prevVel;
	Vector2 _drawOffset;

	// ジャンプ関連変数
	int _jumpFrame;		// ジャンプ長押し時間
	bool _isJumping;	// ジャンプ中フラグ

	bool _isTurn;	// true:左向き/false:右向き
	Vector2 _shotPos;	// 弾を召喚する位置

	bool _isSlide;	// 壁スライド状態かどうか

	int _walkSoundFrame = 0; // 歩行音再生用フレームカウント

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