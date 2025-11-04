#include "Player.h"
#include "Dxlib.h"
#include <cassert>
#include "input.h"
#include "BoxCollider.h"
#include "Bullet.h"

namespace
{
	// 描画関連
	constexpr int PLAYER_GRAPH_CUT_W = 40;	// プレイヤー画像の切り取りサイズ
	constexpr int PLAYER_GRAPH_CUT_H = 40;
	const Vector2 PLAYER_FRAME_SIZE = { PLAYER_GRAPH_CUT_W,PLAYER_GRAPH_CUT_H };
	constexpr float DRAW_SCALE = 3.0f;	// 描画倍率

	constexpr int CHARGE_PARTICLE_GRAPH_CUT_W = 64;	// チャージパーティクル画像の切り取りサイズ
	constexpr int CHARGE_PARTICLE_GRAPH_CUT_H = 64;
	const Vector2 CHARGE_PARTICLE_FRAME_SIZE = { CHARGE_PARTICLE_GRAPH_CUT_W,CHARGE_PARTICLE_GRAPH_CUT_H };

	// アニメーション関連
	constexpr int PLAYER_IDLE_ANIM_MAX_NUM = 5;	// アニメーションの枚数
	constexpr int PLAYER_MOVE_ANIM_MAX_NUM = 8;

	constexpr int CHARGE_PARTICLE_ANIM_MAX_NUM = 7;

	constexpr int ONE_ANIM_FRAME = 6;	// 何フレームでアニメーションを切り替えるか

	// 当たり判定
	constexpr float COLLIDER_W = 60.0f;	// 当たり判定のサイズ
	constexpr float COLLIDER_H = 80.0f;

	constexpr float GROUND_H = 800.0f;	// 地面の高さ(仮)

	// 動きの制御関連
	constexpr float JUMP_POWER = -15.0f;	// ジャンプ力
	constexpr int MAX_JUMP_FRAME = 15;	// ジャンプを長押しできる最大時間

	constexpr float MOVE_SPEED = 2.0f;	// 横移動速度
	constexpr float MAX_MOVE_SPEED = 10.0f;	// 最高移動速度
	constexpr float STOP_SPEED = 0.9f;	// 横移動速度がこれ以下になったら完全に停止する
	constexpr float FRICTION_POWER = 0.7f;	// 自然に止まる力

	// ダッシュ関連
	constexpr int DASH_COOL_TIME = 120;	// ダッシュのクールタイム
	constexpr float DASH_SPEED = 20.0f;	// ダッシュの速度
	constexpr int DASHING_TIME = 15;	// ダッシュの持続時間

	// 射撃関連
	constexpr int SHOT_CHARGE_TIME = 60;	// チャージショットのチャージ時間
}

// アニメーション種類
enum class PlayerAnimType : int
{	// 画像の切り取り位置に対応
	Idle = 0,
	Move,
	Shot,
	Damage,
	GrabWall,
	Dash,
	FlameDash,
	Death,
	Jump,
	Fall = 8
};

Player::Player():
	_playerH(-1),
	_chargeParticleH(-1),
	_jumpFrame(0),
	_isGround(false),
	_isJumping(false),
	_isTurn(false),
	_dashCoolTime(0),
	_dashFrame(0),
	_isDashing(false),
	_isTurnDashing(false),
	_chargeFrame(0),
	_isCharging(false)
{
	_collider = std::make_shared<BoxCollider>(_pos,Vector2(COLLIDER_W, COLLIDER_H));
	_playerAfterimage.resize(3);
	for (auto& afterimage : _playerAfterimage)
	{
		afterimage.frame = 0;
	}
}

Player::~Player()
{
}

void Player::Init()
{
	_pos = { 100.0f,100.0f };
	_ChargeParticleAnim.Init(_chargeParticleH, 0, CHARGE_PARTICLE_FRAME_SIZE, CHARGE_PARTICLE_ANIM_MAX_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	
	_idleAnim.Init(_playerH,static_cast<int>(PlayerAnimType::Idle), PLAYER_FRAME_SIZE, PLAYER_IDLE_ANIM_MAX_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	_moveAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Move), PLAYER_FRAME_SIZE, PLAYER_MOVE_ANIM_MAX_NUM, ONE_ANIM_FRAME, DRAW_SCALE);
	_jumpAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Jump), PLAYER_FRAME_SIZE, 1, ONE_ANIM_FRAME, DRAW_SCALE);
	_fallAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Fall), 2, PLAYER_FRAME_SIZE, DRAW_SCALE);
	_dashAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Dash), 1, PLAYER_FRAME_SIZE, DRAW_SCALE);
	_nowAnim = _idleAnim;
}

void Player::Update()
{
	// 重力をかける
	Gravity();

	// ジャンプ処理
	Jump();

	// 左右移動処理
	Move();

	// ダッシュ処理
	Dash();

	_pos += _vel;	// 速度ベクトルを位置に足しこむ

	if (_pos.y > GROUND_H)	// 位置が地面の高さを上回ったら補正
	{
		_pos.y = GROUND_H;
		_isGround = true;
		_vel.y = 0.0f;
	}

	if (_isTurn)	// 弾を召喚する位置を設定
	{
		_shotPos = { _pos.x - COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}
	else
	{
		_shotPos = { _pos.x + COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}

	// 当たり判定の位置を設定
	_collider->SetPos(_pos);

	// 射撃処理
	Shot();
	ChargeShot();

	if (_dashFrame % 5 == 0 && _isDashing)
	{
		for (auto& afterimage : _playerAfterimage)
		{
			if (afterimage.frame > 10)
			{
				afterimage.frame = 0;
				afterimage.Pos = _pos;
				afterimage.isTurn = _isTurn;
				break;
			}
		}
	}
	
	for (auto& afterimage : _playerAfterimage)
	{
		if (afterimage.frame <= 10)
		{
			afterimage.frame++;
		}
	}
	// アニメーション処理
	UpdateAnim();
}

void Player::Draw()
{
	for (auto& afterimage : _playerAfterimage)
	{
		afterimage.Draw();
	}

	_nowAnim.Draw({ _pos.x, _pos.y - PLAYER_GRAPH_CUT_H / 2 * DRAW_SCALE }, _isTurn);

	// チャージ中にパーティクルを描画
	if (_isCharging)
	{
		_ChargeParticleAnim.Draw({ _pos.x, _pos.y - PLAYER_GRAPH_CUT_H / 2 * DRAW_SCALE },true);
	}
#ifdef _DEBUG
	_collider->Draw();
#endif // _DEBUG
}

void Player::SetHandle(int playerH, int chargeParticleH)
{
	_playerH = playerH;
	_chargeParticleH = chargeParticleH;
	for (auto& afterimage : _playerAfterimage)
	{
		afterimage.handle = playerH;
	}
}

void Player::SetContext(const Input& input, std::vector<std::shared_ptr<Bullet>>& pBullets)
{
	_input = input;
	_pBullets = pBullets;
}

void Player::Jump()
{
	if (_input.IsPressed("jump") && _isGround)
	{	// ジャンプが入力され、かつ接地しているならジャンプ中
		_isJumping = true;
	}
	if (_input.IsPressed("jump"))
	{	// ジャンプが入力され、かつジャンプ中なら長押し有効
		if (_isJumping)
		{
			_jumpFrame++;
			_isGround = false;
			if (_jumpFrame < MAX_JUMP_FRAME)
			{
				_vel.y = JUMP_POWER;
			}
		}
	}
	else
	{	// ジャンプが入力されていないならジャンプ中を解除
		_jumpFrame = 0;
		_isJumping = false;
	}
}

void Player::Move()
{
	if (_input.IsPressed("right"))
	{
		_vel.x += MOVE_SPEED;
		_isTurn = false;
	}
	if (_input.IsPressed("left"))
	{
		_vel.x -= MOVE_SPEED;
		_isTurn = true;
	}

	// 最高速度以上は出ないようにする
	if (_vel.x > MAX_MOVE_SPEED)
	{
		_vel.x = MAX_MOVE_SPEED;
	}
	if (_vel.x < -MAX_MOVE_SPEED)
	{
		_vel.x = -MAX_MOVE_SPEED;
	}

	// 自然に止まる力
	if (_vel.x >= -STOP_SPEED && _vel.x <= STOP_SPEED)
	{
		_vel.x = 0.0f;
	}
	if (_vel.x > STOP_SPEED)
	{
		_vel.x -= FRICTION_POWER;
	}
	if (_vel.x < -STOP_SPEED)
	{
		_vel.x += FRICTION_POWER;
	}
}

void Player::Shot()
{
	if (_input.IsTriggered("shot"))
	{
		for (auto& bullet : _pBullets)
		{
			if (!bullet->GetAlive())
			{
				bullet->Shot(BulletType::NormalShot,_shotPos,_isTurn);
				break;
			}
		}
	}
}

void Player::ChargeShot()
{
	if (_input.IsPressed("shot"))
	{
		_chargeFrame++;
		if (_chargeFrame > 60)
		{
			_isCharging = true;
		}
	}
	else
	{
		if (_chargeFrame > SHOT_CHARGE_TIME)
		{
			for (auto& bullet : _pBullets)
			{
				if (!bullet->GetAlive())
				{
					bullet->Shot(BulletType::ChargeShot, _shotPos, _isTurn);
					break;
				}
			}
		}
		_chargeFrame = 0;
		_isCharging = false;
	}
}

void Player::Dash()
{
	// クールタイムを減らす
	if (_dashCoolTime > 0)
	{
		_dashCoolTime--;
	}
	if (_input.IsTriggered("dash") && _dashCoolTime == 0)
	{	// ダッシュが入力され、かつクールタイムが終わっているなら
		_isTurnDashing = _isTurn;
		_isDashing = true;
		_dashCoolTime = DASH_COOL_TIME;
	}
	if (_isDashing)
	{	// ダッシュ本体の処理
		_dashFrame++;
		_vel.y = 0.0f;
		if (_isTurnDashing)
		{
			_vel.x = -DASH_SPEED;
		}
		else
		{
			_vel.x = DASH_SPEED;
		}
		if (_dashFrame > DASHING_TIME)
		{
			_dashFrame = 0;
			_isDashing = false;
		}
	}
}

void Player::UpdateAnim()
{
	if (_isDashing)
	{
		ChangeAnim(_dashAnim);
	}
	else if (_vel.y < 0)
	{
		ChangeAnim(_jumpAnim);	// ジャンプアニメーションに切り替え
	}
	else if (_vel.y > 0)
	{
		ChangeAnim(_fallAnim);	// 落下アニメーションに切り替え
	}
	else if (abs(_vel.x) > 1.0f)
	{
		ChangeAnim(_moveAnim);	// 移動アニメーションに切り替え
	}
	else
	{
		ChangeAnim(_idleAnim);	// 移動アニメーションに切り替え
	}
	_nowAnim.Update();	// アニメーション更新
	_ChargeParticleAnim.Update(); // チャージパーティクルアニメーション更新
}

void PlayerAfterimage::Draw()
{
	DrawRectRotaGraph(Pos.x,Pos.y - 40,
		40 * 1,40 * 5,
		40,40,
		3.0f,0.0f,handle,isTurn,true);
}