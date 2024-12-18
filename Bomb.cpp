#include "Structure.h"
#include "Constant.h"
#include "Enumeration.h"
#include "Shapes.h"
#include "Matrix.h"
#include "Player.h"
#include "Bomb.h"
#include "Bullet.h"
#include "Particle.h"
#include "Novice.h"

/// <summary>
/// プレイヤーが爆弾を起動する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="bomb">爆弾</param>
void PlayerBombBoot(Player* player, Bomb* bomb)
{
	// null を探す
	if (player == nullptr || bomb == nullptr)
	{
		return;
	}

	// 爆弾を起動する
	for (int i = 0; i < kBombNum; i++)
	{
		if (bomb[i].isBoot == false && bomb[i].isShot == false && bomb[i].respawn.isRespawn)
		{
			// 爆弾を起動する（起動フラグをtrueにする）
			bomb[i].isBoot = true;

			// まだ、発射しない（発射フラグをfalseにする）
			bomb[i].isShot = false;

			// 制限時間
			bomb[i].timeLimit = 180;

			// プレイヤーの上に配置する
			bomb[i].shape = { player->shape.translate.x , player->shape.translate.y + 100.0f };

			// 角度
			bomb[i].shape.theta = 0.0f;

			// 図形の大きさ
			bomb[i].shape.scale = { 16.0f , 16.0f };

			// 移動速度
			bomb[i].vel = { 0.0f , 0.0f };

			// 加速度
			bomb[i].acceleration = { 0.0f , 0.0f };

			break;
		}
	}
}

/// <summary>
/// 爆弾を起動する
/// </summary>
/// <param name="bomb"></param>
void BombBootAndShot(Bomb* bomb , float posX , float posY , float radius)
{
	// null を探す
	if (bomb == nullptr)
	{
		return;
	}

	// 爆弾を起動する
	for (int i = 0; i < kBombNum; i++)
	{
		if (bomb[i].isBoot == false && bomb[i].isShot == false && bomb[i].respawn.isRespawn)
		{
			// 爆弾を起動する（起動フラグをtrueにする）
			bomb[i].isBoot = true;

			// 爆弾を発射する（発射フラグをtrueにする）
			bomb[i].isShot = true;

			// 制限時間
			bomb[i].timeLimit = 180;

			// プレイヤーの上に配置する
			bomb[i].shape = { posX , posY };

			// 角度
			bomb[i].shape.theta = 0.0f;

			// 図形の大きさ
			bomb[i].shape.scale = { radius , radius };

			// 移動速度
			bomb[i].vel = { 0.0f , 4.0f };

			// 加速度
			bomb[i].acceleration = { 0.0f , 0.0f };

			break;
		}
	}
}

/// <summary>
/// 爆弾を発射する
/// </summary>
/// <param name="bomb">爆弾</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="accelerationX">X軸の加速度</param>
/// <param name="accelerationY">Y軸の加速度</param>
void PlayerBombShot(Bomb* bomb, float velX, float velY, float accelerationX, float accelerationY)
{
	// null を探す
	if (bomb == nullptr)
	{
		return;
	}

	for (int i = 0; i < kBombNum; i++)
	{
		if (bomb[i].isBoot && bomb[i].isShot == false && bomb[i].respawn.isRespawn)
		{
			// 爆弾を発射する（発射フラグをtrueにする）
			bomb[i].isShot = true;

			// 移動速度
			bomb[i].vel = { velX , velY };

			// 加速度
			bomb[i].acceleration = { accelerationX , accelerationY };

			break;
		}
	}
}

/// <summary>
/// 爆弾を動かす
/// </summary>
/// <param name="bomb">爆弾</param>
/// <param name="bullet">弾</param>
void BombMove(Bomb* bomb, Bullet* bullet , Particle* particle, SH* sh)
{
	// null を探す
	if (bomb == nullptr || bullet == nullptr || particle == nullptr)
	{
		return;
	}

	for (int i = 0; i < kBombNum; i++)
	{
		if (bomb[i].isShot && bomb[i].isBoot)
		{
			/*   制限時間   */

			bomb[i].timeLimit--;

			// 時間切れで爆発する（起動、発射、復活フラグがfalseになる）
			if (bomb[i].timeLimit <= 0)
			{
				// 爆発する
				BulletShot(bullet, BULLET_TYPE_EXPLOSION, bomb[i].pos.world.leftBottom.x + bomb[i].shape.scale.x, bomb[i].pos.world.leftBottom.y + bomb[i].shape.scale.y);

				ParticleEmission(particle, PARTICLE_TYPE_BOMB, bomb[i].shape.translate.x, bomb[i].shape.translate.y);


				bomb[i].isBoot = false;
				bomb[i].isShot = false;
				bomb[i].respawn.isRespawn = false;

				Novice::PlayAudio(sh->explosive, 0, 0.6f);
			}


			/*   動き   */

			// 浮いているときは、回転する
			if (bomb[i].shape.translate.y > bomb[i].shape.scale.y)
			{
				bomb[i].shape.theta += 3.0f;
			}
			else
			{
				bomb[i].shape.theta = 0.0f;
			}

			// 徐々に落下する
			if (bomb[i].acceleration.y > -2.0f)
			{
				bomb[i].acceleration.y += -0.1f;
			}


			/*   反発   */

			// 地面
			if (bomb[i].shape.translate.y - bomb[i].shape.scale.y <= 0.0f)
			{
				if (bomb[i].acceleration.y < 0.2f && bomb[i].acceleration.y > -0.2f)
				{
					bomb[i].acceleration.y = 0.0f;
				} else
				{
					bomb[i].acceleration.y *= -0.4f;
				}
			}

			// 左の壁
			if (bomb[i].shape.translate.x - bomb[i].shape.scale.x <= 0.0f)
			{
				bomb[i].acceleration.x *= -0.8f;
			}

			// 右の壁
			if (bomb[i].shape.translate.x + bomb[i].shape.scale.x >= static_cast<float>(kScreenWtidh))
			{
				bomb[i].acceleration.x *= -0.8f;
			}

			// 飛距離が縮む
			if (bomb[i].acceleration.x > 0.0f)
			{
				bomb[i].acceleration.x += -0.02f;
			} 
			else if (bomb[i].acceleration.x < 0.0f)
			{
				bomb[i].acceleration.x += 0.02f;
			}

			// 爆弾を動かす
			bomb[i].shape.translate.x += bomb[i].vel.x * bomb[i].acceleration.x;
			bomb[i].shape.translate.y += bomb[i].vel.y * bomb[i].acceleration.y;
			bomb[i].pos.world = VertexAffineMatrix(bomb[i].shape);
		}
	}
}