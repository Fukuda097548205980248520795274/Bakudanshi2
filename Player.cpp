#include <Novice.h>
#include "Structure.h"
#include "Constant.h"
#include "Enumeration.h"
#include "Shapes.h"
#include "Matrix.h"
#include "Player.h"
#include "Bomb.h"
#include "Bullet.h"

/// <summary>
/// ゲーム中のプレイヤーの初期値を入れる
/// </summary>
/// <param name="player"></param>
void PlayerInitialValue(Player* player)
{
	// 復活
	player->respawn.isRespawn = true;
	player->respawn.timer = 120;

	// 体力
	player->damage.hp = 1000;
	player->damage.isDamage = false;
	player->damage.timer = 30;

	// 爆弾を持っているかどうか（爆弾所持フラグ）
	player->isBomHave = false;

	// 向ている方向
	player->directionNo = DIRECTION_RIGHT;

	// 移動速度
	player->vel = { 0.0f , 0.0f };

	// ジャンプ
	player->jump.isJump = false;
	player->jump.count = 0;

	// 図形
	player->shape.scale = { 32.0f , 32.0f };
	player->shape.theta = 0.0f;
	player->shape.translate = { 300.0f , player->shape.scale.y };

	// 位置
	player->pos.world = VertexAffineMatrix(player->shape);
	player->pos.screen = CoordinateTransformation(player->pos.world);
}

/// <summary>
/// プレイヤーの移動操作を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="keys">キー</param>
/// <param name="preKeys">前に押したキー</param>
void PlayerMove(Player* player, const char* keys, const char* preKeys, LeftStick* leftStick, SH* sh)
{

	// nullを探す
	if (player == nullptr || keys == nullptr || preKeys == nullptr || leftStick == nullptr)
	{
		return;
	}


	/*   横移動   */

	// 左スティックを感知する
	Novice::GetAnalogInputLeft(0, &leftStick->x, &leftStick->y);

	// 横移動を初期化する
	player->vel.x = 0.0f;

	// Aキーか左スティックで、左に移動する
	if (keys[DIK_A] || leftStick->x < 0)
	{
		if (player->respawn.isRespawn)
		{
			// 左を向く
			player->directionNo = DIRECTION_LEFT;

			if (player->pos.world.leftTop.x > 0.0f || player->pos.world.leftBottom.x > 0.0f)
			{
				player->vel.x = -5.0f;
			}
		}
	}

	// Dキーか左スティックで、右に移動する
	if (keys[DIK_D] || leftStick->x > 0)
	{
		if (player->respawn.isRespawn)
		{
			// 右を向く
			player->directionNo = DIRECTION_RIGHT;

			if (player->pos.world.rightTop.x < static_cast<int>(kScreenWtidh) ||
				player->pos.world.rightBottom.x < static_cast<int>(kScreenWtidh))
			{
				player->vel.x = 5.0f;
			}
		}
	}

	if (player->vel.x != 0.0f)
	{
		if (!player->jump.isJump) {
			if (!Novice::IsPlayingAudio(sh->pHPlayerWalk) || sh->pHPlayerWalk == -1) {
				sh->pHPlayerWalk = Novice::PlayAudio(sh->playerWalk, 0, 0.8f);
			}
		}
	}

	// 横移動させる
	player->shape.translate.x += player->vel.x;
	player->pos.world = VertexAffineMatrix(player->shape);


	/*   ジャンプ   */

	// Jキーかコントローラーのボタンで、ジャンプする（ジャンプフラグがtrueになる）
	if (!preKeys[DIK_J] && keys[DIK_J] || Novice::IsTriggerButton(0, kPadButton10))
	{
		if (player->respawn.isRespawn)
		{
			// 2段ジャンプまで
			if (player->jump.count < 2)
			{
				player->jump.isJump = true;

				player->jump.count++;

				player->vel.y = 10.0f;

				// 加速度で上に飛ぶ
				player->jump.fallingVel = -0.5f;
				Novice::PlayAudio(sh->jump, 0, 0.15f);
			}
		}
	}

	// ジャンプしている（ジャンプフラグがtrueである）とき、跳躍、落下する
	if (player->jump.isJump)
	{
		if (player->vel.y > -16.0f)
		{
			player->vel.y += player->jump.fallingVel;
		}

		// 縦移動する
		player->shape.translate.y += player->vel.y;
		player->pos.world = VertexAffineMatrix(player->shape);

		// 地面についたら、ジャンプを止める（ジャンプフラグがfalseになる）
		if (player->pos.world.leftBottom.y <= 0.0f || player->pos.world.rightBottom.y <= 0.0f)
		{
			player->jump.isJump = false;
			player->jump.count = 0;
			player->jump.fallingVel = 0.0f;
			Novice::PlayAudio(sh->land, 0, 0.4f);
		}
	}


	/*   位置を調整する   */

	// 地面
	if (player->shape.translate.y - player->shape.scale.y < 0.0f)
	{
		player->shape.translate.y = player->shape.scale.y;
		player->pos.world = VertexAffineMatrix(player->shape);
	}

	// 左の壁
	if (player->shape.translate.x - player->shape.scale.x < 0.0f)
	{
		player->shape.translate.x = player->shape.scale.x;
		player->pos.world = VertexAffineMatrix(player->shape);
	}

	// 右の壁
	if (player->shape.translate.x + player->shape.scale.x > static_cast<float>(kScreenWtidh))
	{
		player->shape.translate.x = static_cast<float>(kScreenWtidh) - player->shape.scale.x;
		player->pos.world = VertexAffineMatrix(player->shape);
	}
}

/// <summary>
/// プレイヤーが爆弾を使う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="bomb">爆弾</param>
/// <param name="bullet">弾</param>
/// <param name="keys">キー</param>
/// <param name="preKeys">前に押したキー</param>
void PlayerBombUse(Player* player, Bomb* bomb, Bullet* bullet, const char* keys, const char* preKeys, SH* sh)
{
	// nullを探す
	if (player == nullptr || bomb == nullptr || keys == nullptr || preKeys == nullptr)
	{
		return;
	}

	// スペースキーかコントローラーのボタンで、爆弾を使う
	if (!preKeys[DIK_SPACE] && keys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton12))
	{
		if (player->respawn.isRespawn)
		{
			// 爆弾を持っていない（爆弾所持フラグがfalseである）ときは、爆弾を起動する
			if (player->isBomHave == false)
			{
				// 爆弾を所持する（爆弾所持フラグがtrueになる）
				player->isBomHave = true;

				// 爆弾を起動する
				PlayerBombBoot(player, bomb);

				if (!Novice::IsPlayingAudio(sh->pHBomCount) || sh->pHBomCount == -1) {
					sh->pHBomCount = Novice::PlayAudio(sh->bomCount, 0, 0.6f);
				}
			}
			else
			{
				// 爆弾を持っている（爆弾所持フラグがtrueである）ときは、爆弾を発射する

				if (player->directionNo == DIRECTION_LEFT)
				{
					if (player->vel.x != 0.0f)
					{
						PlayerBombShot(bomb, 3.0f, 4.0f, -2.0f, 1.0f);
					} else
					{
						PlayerBombShot(bomb, 3.0f, 4.0f, -1.0f, 1.0f);
					}
				} else if (player->directionNo == DIRECTION_RIGHT)
				{
					if (player->vel.x != 0.0f)
					{
						PlayerBombShot(bomb, 3.0f, 4.0f, 2.0f, 1.0f);
					} else
					{
						PlayerBombShot(bomb, 3.0f, 4.0f, 1.0f, 1.0f);
					}
				}

				// プレイヤーが爆弾を離す（爆弾所持フラグをfalseにする）
				player->isBomHave = false;

				Novice::StopAudio(sh->pHBomCount);
				sh->volumeBomCount = 0.4f;
			}
		}
	}

	// プレイヤーが爆弾を持つ（爆弾所持フラグがtrueであるときの動き）
	if (player->isBomHave)
	{
		// 起動している（起動フラグがtrueである）爆弾を持つ
		for (int i = 0; i < kBombNum; i++)
		{
			if (bomb[i].isBoot && bomb[i].isShot == false)
			{
				// プレイヤーの上に配置する
				bomb[i].shape.translate = { player->shape.translate.x , player->shape.translate.y + 50.0f };
				bomb[i].pos.world = VertexAffineMatrix(bomb[i].shape);


				/*   制限時間   */

				bomb[i].timeLimit--;

				// 時間切れで爆発する（起動、発射、復活フラグがfalseになる）
				if (bomb[i].timeLimit <= 0)
				{
					// 爆発する
					BulletShot(bullet, BULLET_TYPE_EXPLOSION, bomb[i].pos.world.leftBottom.x + bomb[i].shape.scale.x, bomb[i].pos.world.leftBottom.y + bomb[i].shape.scale.y);

					bomb[i].isBoot = false;
					bomb[i].isShot = false;
					bomb[i].respawn.isRespawn = false;

					// プレイヤーが爆弾を離す（爆弾所持フラグをfalseにする）
					player->isBomHave = false;

					Novice::StopAudio(sh->pHBomCount);
					Novice::PlayAudio(sh->explosive, 0, 0.6f);
					sh->volumeBomCount = 0.4f;
				}
			}
		}

		sh->volumeBomCount += 0.05f;
		Novice::SetAudioVolume(sh->pHBomCount, sh->volumeBomCount);

	}
}