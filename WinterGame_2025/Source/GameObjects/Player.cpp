#include "Player.h"
#include "Dxlib.h"
#include <cassert>
#include "../Systems/input.h"
#include "../Utility/BoxCollider.h"
#include "Bullet.h"
#include "../Systems/Map.h"

namespace
{
	// 描画関連
	constexpr int kGraphCutW = 40;	// プレイヤー画像の切り取りサイズ
	constexpr int kGraphCutH = 40;
	const Vector2 kFrameSize = { kGraphCutW,kGraphCutH };
	constexpr float kDrawScale = 3.0f;	// 描画倍率

	constexpr int kChargeParticleGraphCutW = 64;	// チャージパーティクル画像の切り取りサイズ
	constexpr int kChargeParticleGraphCutH = 64;
	const Vector2 kChargeParticleFrameSize = { kChargeParticleGraphCutW,kChargeParticleGraphCutH };

	const Vector2 kShotFlashGraphSize = { 16,16 };	// 通常ショットのマズルフラッシュのサイズ
	const Vector2 kChargeShotFlashGraphSize = { 32,32 };	// チャージショットのマズルフラッシュのサイズ

	// アニメーション関連
	constexpr int kIdleAnimNum = 5;	// アニメーションの枚数
	constexpr int kMoveAnimNum = 8;
	constexpr int kDamageAnimNum = 9;
	constexpr int kDeathAnimNum = 21;

	constexpr int kChargeParticleAnimNum = 7;

	constexpr int kShotAnimNum = 3;
	constexpr int kChargeShotAnimNum = 4;

	constexpr int kOneAnimFrame = 6;	// 何フレームでアニメーションを切り替えるか
	constexpr int kDeathOneAnimFrame = 7;	// 死亡時のアニメーション用
	constexpr int kFlashOneAnimFrame = 4;	// 通常ショットのマズルフラッシュ
	constexpr int kChargeShotFlashOneAnimFrame = 6;	// チャージショットのマズルフラッシュ

	// 当たり判定
	constexpr float kColliderW = 60.0f;	// 当たり判定のサイズ
	constexpr float kColliderH = 80.0f;

	constexpr float kGroundH = 800.0f;	// 地面の高さ(仮)

	// ダメージ関連
	constexpr int kInvincibleMaxFrame = 180;	// ダメージを受けてからの無敵時間
	constexpr float kKnockbackPowerX = 13.0f;	// ダメージを受けたときのノックバックの力X
	constexpr float kKnockbackPowerY = 10.0f;	// ダメージを受けたときのノックバックの力Y
	constexpr int kDamageAnimEndFrame = kInvincibleMaxFrame - kDamageAnimNum * kOneAnimFrame;	// ダメージアニメーションが終わるフレーム
	constexpr int kFlickerInterval = 5;	// 点滅の間隔

	// 動きの制御関連
	constexpr float kJumpPower = -15.0f;	// ジャンプ力
	constexpr int kMaxJumpFrame = 15;	// ジャンプを長押しできる最大時間

	constexpr float kMoveSpeed = 2.0f;	// 横移動速度
	constexpr float kMaxMoveSpeed = 10.0f;	// 最高移動速度
	constexpr float kStopSpeed = 0.9f;	// 横移動速度がこれ以下になったら完全に停止する
	constexpr float kFrictionPower = 0.7f;	// 自然に止まる力

	// ダッシュ関連
	constexpr int kDashCoolTime = 120;	// ダッシュのクールタイム
	constexpr float kDashSpeed = 20.0f;	// ダッシュの速度
	constexpr int kDashingTime = 15;	// ダッシュの持続時間
	constexpr int kDashGraphIndexX = 1;	// ダッシュ時の画像の切り取り位置X
	constexpr int kDashGraphIndexY = 5;	// ダッシュ時の画像の切り取り位置Y

	constexpr int kAfterimageNum = 4;	// 残像の数
	constexpr int kAfterimageFrameMax = 10;	// 残像が消えるまでのフレーム数
	constexpr int kAfterimageFrameInterval = 3;	// 残像が出る間隔
	constexpr int kAfterimageColorR = 64;	// 残像の色
	constexpr int kAfterimageColorG = 64;
	constexpr int kAfterimageColorB = 255;

	// 射撃関連
	constexpr int kChargeEffectTime = 30;		// チャージエフェクトが出始める時間
	constexpr int kChargeTimeMax = 90;	// 最大チャージ時間

	constexpr int kMaxHp = 5;
}

// アニメーション種類
enum class PlayerAnimType : int
{	// 画像の切り取り位置に対応
	Idle = 0,
	Move,
	Shot,
	Damage,
	Slide,
	Dash,
	FlameDash,
	Death,
	Jump,
	Fall = 8
};

Player::Player(int playerH, int playerWhiteH, int chargeParticleH,int shotH,int chargeShotH):
	_playerH(playerH),
	_playerWhiteH(playerWhiteH),
	_chargeParticleH(chargeParticleH),
	_shotH(shotH),
	_chargeShotH(chargeShotH),
	_hp(kMaxHp),
	_isAlive(true),
	_jumpFrame(0),
	_isJumping(false),
	_isTurn(false),
	_isSlide(false),
	_isCanControll(true),
	_invincibleFrame(0),
	_isFrickering(false),
	_dashCoolTime(0),
	_dashFrame(0),
	_isDashing(false),
	_isTurnDashing(false),
	_chargeFrame(0)
{
#ifdef _DEBUG
	_isCanFly = false;
#endif
	_pCollider = std::make_shared<BoxCollider>(_pos,Vector2(kColliderW, kColliderH));
	_playerAfterimage.resize(kAfterimageNum);
	for (auto& afterimage : _playerAfterimage)
	{
		afterimage.frame = kAfterimageFrameMax + 1;
		afterimage.handle = _playerH;
		afterimage.whiteHandle = _playerWhiteH;
	}

	_idleAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Idle), kFrameSize, kIdleAnimNum, kOneAnimFrame, kDrawScale);
	_moveAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Move), kFrameSize, kMoveAnimNum, kOneAnimFrame, kDrawScale);
	_damageAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Damage), kFrameSize, kDamageAnimNum, kOneAnimFrame, kDrawScale);
	_jumpAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Jump), kFrameSize, 1, kOneAnimFrame, kDrawScale);
	_fallAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Fall), 2, kFrameSize, kDrawScale);
	_dashAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Dash), 1, kFrameSize, kDrawScale);
	_slideAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Slide), 0, kFrameSize, kDrawScale);
	_deathAnim.Init(_playerH, static_cast<int>(PlayerAnimType::Death), kFrameSize, kDeathAnimNum, kDeathOneAnimFrame, kDrawScale, false);

	_shotFlashAnim.Init(_shotH, 0, kShotFlashGraphSize, kShotAnimNum, kFlashOneAnimFrame, kDrawScale,false);
	_shotFlashAnim.SetEnd();
	_chargeShotFlashAnim.Init(_chargeShotH, 0, kChargeShotFlashGraphSize, kChargeShotAnimNum, kChargeShotFlashOneAnimFrame, kDrawScale,false);
	_chargeShotFlashAnim.SetEnd();
	_ChargeParticleAnim.Init(_chargeParticleH, 0, kChargeParticleFrameSize, kChargeParticleAnimNum, kOneAnimFrame, kDrawScale);

	_nowAnim = _idleAnim;
}

Player::~Player()
{
}

void Player::Init()
{
	
}

void Player::Update(Map& map)
{
	// 重力をかける
	Gravity();

	if (_isCanControll)
	{
		// ジャンプ処理
		Jump();

		// 左右移動処理
		Move();

		// 壁ずり落ち
		Slide();

		// ダッシュ処理
		Dash();

		// 射撃処理
		if (_isTurn)	// 弾を召喚する位置を設定
		{
			_shotPos = { _pos.x - kColliderW / 2, _pos.y - kColliderH / 2 };
		}
		else
		{
			_shotPos = { _pos.x + kColliderW / 2, _pos.y - kColliderH / 2 };
		}
		Shot();
		ChargeShot();
	}
	if (!_isDashing && _invincibleFrame < kDamageAnimEndFrame)	// ダッシュ中でなく、被弾アニメーション中でなければ
	{
		// 移動速度制限処理
		MoveSpeedLimit();
	}

	// 移動抵抗処理
	MoveResistance();

	// 落下中なら接地フラグをfalseにする
	if (_vel.y > 0)
	{
		_isGround = false;
	}

	// マップとの当たり判定処理
	_hitDir = MapCollision(map);

	// hpがなくなったときの処理
	if (_hp <= 0)
	{
		_isCanControll = false;
		if (_nowAnim.GetIsEnd())
		{
			// ここに死んだときの処理を書く
			_isAlive = false;
		}
	}

	// ダメージを受けているときの処理
	DamageUpdate();

	// 画面外に出ないようにする
	MoveAreaLimit(map);

	// 残像の更新
	UpdateAfterimage();

	// アニメーション処理
	UpdateAnim();

#ifdef _DEBUG
	// 飛行モードの切り替え【デバッグ用】
	if (CheckHitKey(KEY_INPUT_M))
	{
		_isCanFly = true;
	}
	if (CheckHitKey(KEY_INPUT_N))
	{
		_isCanFly = false;
	}
	if (_isCanFly)
	{
		if (_input.IsPressed("up") || _input.IsPressed("jump"))
		{
			_vel.y = -10.0f;
		}
		if (_input.IsPressed("down"))
		{
			_vel.y = 10.0f;
		}
		if (_input.IsPressed("left"))
		{
			_vel.x = -10.0f;
		}
		if (_input.IsPressed("right"))
		{
			_vel.x = 10.0f;
		}
	}

	// hpを0にする
	if (CheckHitKey(KEY_INPUT_B))
	{
		_hp = 0;
		// 点滅中に強制的にhpを0にすると非表示のまま死ぬ場合があるので点滅を解除
		_isFrickering = false;
	}
#endif
}

void Player::Draw(Vector2 offset)
{
	// 残像の描画
	for (auto& afterimage : _playerAfterimage)
	{
		afterimage.Draw(offset);
	}

	// プレイヤー本体の描画
	Vector2 drawPos = { _pos.x - offset.x, _pos.y - kGraphCutH / 2 * kDrawScale - offset.y };
	if (_dashCoolTime > 0 && _invincibleFrame == 0)	// ダッシュがクールタイム中なら
	{	// 少し暗く青みがかった色で描画
		SetDrawBright(0xaa, 0xaa, 0xdd);
	}
	if (_isFrickering)	// 点滅中なら
	{	// 一定間隔で描画・非描画を切り替える
		if (_invincibleFrame % kFlickerInterval * 2 < kFlickerInterval)
		{
			_nowAnim.Draw(drawPos, _isTurn);
		}
	}
	else
	{	// 通常描画
		_nowAnim.Draw(drawPos, _isTurn);
	}
	
	SetDrawBright(255, 255, 255);	// 元の色に戻す

	// マズルフラッシュの描画
	constexpr int shotFlashOffset = 30;
	constexpr int chargeShotFlashOffset = 20;
	Vector2 flashPos = _shotPos;
	int temp = 0;
	if (_isTurn)
	{
		temp = -1;
	}
	else
	{
		temp = 1;
	}
	flashPos.x += shotFlashOffset * temp;
	_shotFlashAnim.Draw(flashPos - offset, _isTurn);
	flashPos.x += chargeShotFlashOffset * temp;
	_chargeShotFlashAnim.Draw(flashPos - offset, _isTurn);

	// チャージエフェクトを描画
	if (_chargeFrame > kChargeEffectTime)
	{
		_ChargeParticleAnim.Draw(drawPos, true);
	}
	// チャージが完了したら白フィルターを点滅させる
	if (_chargeFrame >= kChargeTimeMax)
	{
		if (_chargeFrame % kFlickerInterval * 2 < kFlickerInterval)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			_nowAnim.Draw(_playerWhiteH, drawPos, _isTurn);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
#ifdef _DEBUG
	_pCollider->Draw(offset);

	//DrawFormatString(_pos.x - offset.x - 20, _pos.y - offset.y - 120, 0xffffff, "hp:%d", _hp);
	if (_isCanFly)
	{
		DrawFormatString(_pos.x - offset.x - 20, _pos.y - offset.y - 120, 0xffffff, "canFly");
	}
#endif // _DEBUG
}

void Player::SetContext(const Input& input, std::vector<std::shared_ptr<Bullet>>& pBullets)
{
	_input = input;
	_pBullets = pBullets;
}

void Player::TakeDamage()
{
	if (!_isDashing && _invincibleFrame == 0)
	{	// ダッシュ中でなく、無敵時間でなければダメージを受ける
		_hp--;
		_invincibleFrame = kInvincibleMaxFrame;	// 無敵時間をセット
		_isCanControll = false;	// 操作不可にする
		_isFrickering = false;	// 点滅フラグをリセット
		_vel.y = -kKnockbackPowerY;	// 上方向に吹き飛ぶ
		_chargeFrame = 0;	// チャージフレームをリセット
		if (_isTurn)	// ダメージを受けた方向と逆に吹き飛ぶ
		{
			_vel.x = kKnockbackPowerX;
		}
		else
		{
			_vel.x = -kKnockbackPowerX;
		}
	}
}

int Player::GetMaxHp()
{
	return kMaxHp;
}

void Player::Jump()
{
	if (_input.IsPressed("jump") && _isGround)
	{	// ジャンプが入力され、かつ接地しているならジャンプ中
		_isJumping = true;
	}

	if (_input.IsPressed("jump") && _isJumping)
	{	
		if (_hitDir.up)
		{
			_jumpFrame = kMaxJumpFrame;	// 天井に当たったらジャンプ力を加えないようにする
		}
		
		// ジャンプが入力されており、かつジャンプ中ならジャンプ力を加える
		_jumpFrame++;
		_isGround = false;
		if (_jumpFrame < kMaxJumpFrame)
		{
			_vel.y = kJumpPower;
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
	// 左右移動の入力処理
	if (_input.IsPressed("right"))
	{
		_vel.x += kMoveSpeed;
		if (!_isDashing) _isTurn = false;
	}
	if (_input.IsPressed("left"))
	{
		_vel.x -= kMoveSpeed;
		if (!_isDashing) _isTurn = true;
	}
}

void Player::Slide()
{
	// 壁ずり落ち判定
	if ((_hitDir.left || _hitDir.right) && !_hitDir.down && _vel.y > 0)
	{
		_isSlide = true;
	}
	else
	{
		_isSlide = false;
	}

	// 壁ずり落ち中の処理
	if (_isSlide)
	{
		// 落下速度制限
		if (_vel.y > 4.0f)
		{
			_vel.y = 4.0f;
		}

		if (_hitDir.left)
		{
			_isTurn = false;	// 右向きにする
		}
		else if (_hitDir.right)
		{
			_isTurn = true;	// 左向きにする
		}

		// 壁ジャンプ処理
		if (_input.IsTriggered("jump") && _hitDir.left)
		{	// 壁ジャンプ(左壁)
			_vel.y = kJumpPower;
			_vel.x = 15;
			_isSlide = false;
		}
		else if (_input.IsTriggered("jump") && _hitDir.right)
		{	// 壁ジャンプ(右壁)
			_vel.y = kJumpPower;
			_vel.x = -15;
			_isSlide = false;
		}
	}
}

void Player::MoveSpeedLimit()
{
	// 最高速度以上は出ないようにする
	if (_vel.x > kMaxMoveSpeed)
	{
		_vel.x = kMaxMoveSpeed;
	}
	if (_vel.x < -kMaxMoveSpeed)
	{
		_vel.x = -kMaxMoveSpeed;
	}
}

void Player::MoveResistance()
{
	// 自然に止まる力
	if (_vel.x >= -kStopSpeed && _vel.x <= kStopSpeed)
	{
		_vel.x = 0.0f;
	}
	if (_vel.x > kStopSpeed)
	{
		_vel.x -= kFrictionPower;
	}
	if (_vel.x < -kStopSpeed)
	{
		_vel.x += kFrictionPower;
	}
}

void Player::MoveAreaLimit(Map& map)
{
	if (_pos.x < kColliderW / 2)
	{
		_pos.x = kColliderW / 2;
		_vel.x = 0.0f;
	}
	if (_pos.x > map.GetStageWidth() - kColliderW / 2)
	{
		_pos.x = map.GetStageWidth() - kColliderW / 2;
		_vel.x = 0.0f;
	}
}

void Player::DamageUpdate()
{
	if (_hp <= 0) return;	// 死んでいる場合は処理しない
	if (_invincibleFrame > 0)
	{
		_invincibleFrame--;
		if (_invincibleFrame == kDamageAnimEndFrame)
		{	// 被弾アニメーションが終わったら操作可能にする
			_isCanControll = true;
			_isFrickering = true;
		}
	}
	else if (_invincibleFrame <= 0)
	{	// 無敵時間が終わったら点滅終了
		_isFrickering = false;
	}
}

void Player::Shot()
{
	// 通常射撃処理
	if (_input.IsTriggered("shot"))
	{	
		for (auto& bullet : _pBullets)
		{	// 空いている弾を探して発射
			if (!bullet->GetAlive())
			{
				bullet->Shot(BulletType::NormalShot,_shotPos,_isTurn);
				_shotFlashAnim.SetFirst();
				break;
			}
		}
	}
}

void Player::ChargeShot()
{	// チャージショット処理
	if (_input.IsPressed("shot"))
	{	// チャージする
		_chargeFrame++;
	}
	else
	{	
		if (_chargeFrame > kChargeTimeMax)
		{	// チャージが完了しているならチャージショットを発射
			for (auto& bullet : _pBullets)
			{
				if (!bullet->GetAlive())
				{
					bullet->Shot(BulletType::ChargeShot, _shotPos, _isTurn);
					_chargeShotFlashAnim.SetFirst();
					break;
				}
			}
		}
		else if (_chargeFrame > kChargeEffectTime)
		{	// チャージが未完了なら通常弾を発射
			for (auto& bullet : _pBullets)
			{
				if (!bullet->GetAlive())
				{
					bullet->Shot(BulletType::NormalShot, _shotPos, _isTurn);
					_shotFlashAnim.SetFirst();
					break;
				}
			}
		}
		_chargeFrame = 0;
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
		_dashCoolTime = kDashCoolTime;
	}
	if (_isDashing)
	{	// ダッシュ本体の処理
		_dashFrame++;
		_vel.y = 0.0f;
		if (_isTurnDashing)
		{
			_vel.x = -kDashSpeed;
		}
		else
		{
			_vel.x = kDashSpeed;
		}
		if (_dashFrame > kDashingTime)
		{
			_dashFrame = 0;
			_isDashing = false;
		}
	}
}

void Player::UpdateAnim()
{
	if (_hp <= 0)
	{
		ChangeAnim(_deathAnim);	// hpがなくなったら死亡アニメーションに切り替え
	}
	else if (_invincibleFrame > kDamageAnimEndFrame)	// 無敵時間中にダメージアニメーション一回分の時間ダメージアニメーションに切り替え
	{
		ChangeAnim(_damageAnim);
	}
	else if (_isDashing)	// ダッシュ中ならダッシュアニメーションに切り替え
	{
		ChangeAnim(_dashAnim);
	}
	else if (_vel.y < 0)	// 上昇中ならジャンプアニメーションに切り替え
	{
		ChangeAnim(_jumpAnim);
	}
	else if (_isSlide)	// 壁ずり落ち中なら壁ずり落ちアニメーションに切り替え
	{
		ChangeAnim(_slideAnim);
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
	_shotFlashAnim.Update();
	_chargeShotFlashAnim.Update();
}

void Player::UpdateAfterimage()
{
	if (_dashFrame % kAfterimageFrameInterval == 0 && _isDashing)	// ダッシュ中、一定間隔で残像を出す
	{	// 空いている残像データに情報をセット
		for (auto& afterimage : _playerAfterimage)
		{	// フレームが最大値を超えているものを探す
			if (afterimage.frame > kAfterimageFrameMax)
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
		if (afterimage.frame <= kAfterimageFrameMax)
		{
			afterimage.frame++;
		}
	}
}

void Player::PlayerAfterimage::Draw(Vector2 offset)
{
	// 透明度を計算して描画
	float alpha = static_cast<float>(frame) / (kAfterimageFrameMax + 1);	// 透明度の割合
	alpha = 1 - alpha;	// 逆転させる
	alpha *= 255 / 2;	// 0~128の範囲に変換

	SetDrawBright(kAfterimageColorR, kAfterimageColorG, kAfterimageColorB);	// 残像の色を設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));	// 透明度を設定

	DrawRectRotaGraph(pos.x - offset.x, pos.y - kGraphCutH / 2 * kDrawScale - offset.y,	// 残像本体を描画
		kGraphCutW * kDashGraphIndexX, kGraphCutH * kDashGraphIndexY,
		kGraphCutW, kGraphCutH,
		kDrawScale, 0.0f, handle, true, isTurn);

	SetDrawBright(255, 255, 255);	// 色を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// 透明を元に戻す
}