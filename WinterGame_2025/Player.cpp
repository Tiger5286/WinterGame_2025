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

	constexpr int AFTERIMAGE_NUM = 4;	// 残像の数
	constexpr int AFTERIMAGE_FRAME_MAX = 10;	// 残像が消えるまでのフレーム数
	constexpr int AFTERIMAGE_FRAME_INTERVAL = 3;	// 残像が出る間隔
	constexpr int AFTERIMAGE_COLOR_R = 32;	// 残像の色
	constexpr int AFTERIMAGE_COLOR_G = 32;
	constexpr int AFTERIMAGE_COLOR_B = 255;

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
	_playerAfterimage.resize(AFTERIMAGE_NUM);
	for (auto& afterimage : _playerAfterimage)
	{
		afterimage.frame = AFTERIMAGE_FRAME_MAX + 1;
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

	// 当たり判定の位置を設定
	_collider->SetPos(_pos);

	// 射撃処理
	if (_isTurn)	// 弾を召喚する位置を設定
	{
		_shotPos = { _pos.x - COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}
	else
	{
		_shotPos = { _pos.x + COLLIDER_W / 2, _pos.y - COLLIDER_H / 2 };
	}
	Shot();
	ChargeShot();

	// 残像の更新
	UpdateAfterimage();

	// アニメーション処理
	UpdateAnim();
}

void Player::Draw()
{
	// 残像の描画
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
		assert(afterimage.handle != -1);
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
		if (!_isDashing) _isTurn = false;
	}
	if (_input.IsPressed("left"))
	{
		_vel.x -= MOVE_SPEED;
		if (!_isDashing) _isTurn = true;
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
		if (_chargeFrame > SHOT_CHARGE_TIME)
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
	if (_isDashing)	// ダッシュ中ならダッシュアニメーションに切り替え
	{
		ChangeAnim(_dashAnim);
	}
	else if (_vel.y < 0)	// 上昇中ならジャンプアニメーションに切り替え
	{
		ChangeAnim(_jumpAnim);
	}
	else if (_vel.y > 0)	// 下降中なら落下アニメーションに切り替え
	{
		ChangeAnim(_fallAnim);
	}
	else if (abs(_vel.x) > 1.0f)	// 横移動中なら移動アニメーションに切り替え
	{
		ChangeAnim(_moveAnim);
	}
	else	// それ以外は待機アニメーションに切り替え
	{
		ChangeAnim(_idleAnim);
	}
	_nowAnim.Update();	// アニメーション更新
	_ChargeParticleAnim.Update(); // チャージパーティクルアニメーション更新
}

void Player::UpdateAfterimage()
{
	if (_dashFrame % AFTERIMAGE_FRAME_INTERVAL == 0 && _isDashing)	// ダッシュ中、一定間隔で残像を出す
	{	// 空いている残像データに情報をセット
		for (auto& afterimage : _playerAfterimage)
		{	// フレームが最大値を超えているものを探す
			if (afterimage.frame > AFTERIMAGE_FRAME_MAX)
			{
				afterimage.isTurn = _isTurn;
				afterimage.frame = 0;
				afterimage.pos = _pos;
				break;
			}
		}
	}

	for (auto& afterimage : _playerAfterimage)	// 残像のフレームを進める
	{	// 最大値以下ならフレームを進める
		if (afterimage.frame <= AFTERIMAGE_FRAME_MAX)
		{
			afterimage.frame++;
		}
	}
}

void Player::PlayerAfterimage::Draw()
{
	// 透明度を計算して描画
	float alpha = static_cast<float>(frame) / (AFTERIMAGE_FRAME_MAX + 1);	// 透明度の割合
	alpha = 1 - alpha;	// 逆転させる
	alpha *= 255;	// 0~255の範囲に変換
	SetDrawBright(AFTERIMAGE_COLOR_R, AFTERIMAGE_COLOR_G, AFTERIMAGE_COLOR_B);	// 残像の色を設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));	// 透明度を設定
	DrawRectRotaGraph(pos.x, pos.y - 40 / 2 * 3,	// 描画処理
		40 * 1,40 * 5,
		40,40,
		3.0f,0.0f,handle,true, isTurn);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// 元に戻す
	SetDrawBright(255, 255, 255);
}