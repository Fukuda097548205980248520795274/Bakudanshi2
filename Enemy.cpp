#define _USE_MATH_DEFINES
#include <math.h>
#include "Structure.h"
#include "Constant.h"
#include "Enumeration.h"
#include "Shapes.h"
#include "Matrix.h"
#include "Player.h"
#include "Bomb.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Particle.h"
#include "Novice.h"

/// <summary>
/// 敵を出現させる
/// </summary>
/// <param name="enemy">敵</param>
/// <param name="type">種類</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
void EnemyArrival(Enemy* enemy, int type, float posX, float posY, SH* sh)
{
	// nullを探す
	if (enemy == nullptr)
	{
		return;
	}

	// 敵を出現させる
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (enemy[i].isArrival == false && enemy[i].respawn.isRespawn)
		{
			// 敵を出現させる（出現フラグがtrueになる）1
			enemy[i].isArrival = true;

			// フレーム
			enemy[i].frame = 0;

			// 種類
			enemy[i].type = type;

			// 向いている方向
			enemy[i].directionNo = DIRECTION_LEFT;

			switch (enemy[i].type)
			{
			case ENEMY_TYPE_GROUND:

				// ダメージ
				enemy[i].damage.hp = 3;
				enemy[i].damage.isDamage = false;
				enemy[i].damage.timer = 30;

				// 図形
				enemy[i].shape.scale = { 16.0f , 16.0f };
				enemy[i].shape.theta = 0.0f;
				enemy[i].shape.translate = { posX , posY };

				// ベクトル
				enemy[i].vec = { 0.0f , 0.0f };

				// 移動速度
				enemy[i].vel = { 0.0f , 0.0f };

				if (!Novice::IsPlayingAudio(sh->pHEnemyStart) || sh->pHEnemyStart == -1) {
					sh->pHEnemyStart = Novice::PlayAudio(sh->enemyStart, 0, 0.8f);
				}

				break;

			case ENEMY_TYPE_FLYING:

				// ダメージ
				enemy[i].damage.hp = 3;
				enemy[i].damage.isDamage = false;
				enemy[i].damage.timer = 30;

				// 図形
				enemy[i].shape.scale = { 16.0f , 16.0f };
				enemy[i].shape.theta = 0.0f;
				enemy[i].shape.translate = { posX , posY };

				// ベクトル
				enemy[i].vec = { 0.0f , 1.0f };

				// 移動速度
				enemy[i].vel = { 1.0f , 1.0f };

				Novice::PlayAudio(sh->flyingEnemyStart, 0, 0.7f);

				break;
			}

			// 位置
			enemy[i].pos.world = VertexAffineMatrix(enemy[i].shape);
			enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);

			break;
		}
	}
}

/// <summary>
/// 敵を動かす
/// </summary>
/// <param name="enemy">敵</param>
void EnemyMove(Enemy* enemy, Player* player , Particle* particle)
{
	// null を探す
	if (enemy == nullptr || player == nullptr)
	{
		return;
	}

	// 出現している（出現フラグがtrueである）敵を動かす
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (enemy[i].isArrival)
		{

			// フレームを進める
			enemy[i].frame++;

			switch (enemy[i].type)
			{
			case ENEMY_TYPE_GROUND:

				if (enemy[i].frame >= 30)
				{
					if (enemy[i].directionNo == DIRECTION_LEFT)
					{
						if (enemy[i].vel.x > -1.0f)
						{
							enemy[i].vel.x += -0.1f;
						}
					} else if (enemy[i].directionNo == DIRECTION_RIGHT)
					{
						if (enemy[i].vel.x < 1.0f)
						{
							enemy[i].vel.x += 0.1f;
						}
					}
				}

				if (enemy[i].frame >= 1200)
				{
					enemy[i].respawn.isRespawn = false;
					enemy[i].isArrival = false;
				}

				enemy[i].shape.translate.x += enemy[i].vel.x;
				enemy[i].pos.world = VertexAffineMatrix(enemy[i].shape);

				break;

			case ENEMY_TYPE_FLYING:

				if (enemy[i].frame % 5 == 0)
				{
					ParticleEmission(particle, PARTICLE_TYPE_CLOUD, enemy[i].shape.translate.x, enemy[i].shape.translate.y);
				}

				// 敵からプレイヤーのベクトル
				Vec2 enemyToPlayer;
				enemyToPlayer = { player->shape.translate.x - enemy[i].shape.translate.x , player->shape.translate.y - enemy[i].shape.translate.y };

				float length = sqrtf(powf(enemyToPlayer.x, 2) + powf(enemyToPlayer.y, 2));

				if (length != 0.0f)
				{
					enemyToPlayer.x = enemyToPlayer.x / length;
					enemyToPlayer.y = enemyToPlayer.y / length;
				}

				// 外積
				float crossProduct = enemy[i].vec.x * enemyToPlayer.y - enemy[i].vec.y * enemyToPlayer.x;

				if (crossProduct < -0.1f)
				{
					enemy[i].vec.x = enemy[i].vec.x * cosf((-2.0f / 180.0f) * float(M_PI)) - enemy[i].vec.y * sinf((-2.0f / 180.0f) * float(M_PI));
					enemy[i].vec.y = enemy[i].vec.x * sinf((-2.0f / 180.0f) * float(M_PI)) + enemy[i].vec.y * cosf((-2.0f / 180.0f) * float(M_PI));
				} else if (crossProduct > 0.1f)
				{
					enemy[i].vec.x = enemy[i].vec.x * cosf((2.0f / 180.0f) * float(M_PI)) - enemy[i].vec.y * sinf((2.0f / 180.0f) * float(M_PI));
					enemy[i].vec.y = enemy[i].vec.x * sinf((2.0f / 180.0f) * float(M_PI)) + enemy[i].vec.y * cosf((2.0f / 180.0f) * float(M_PI));
				}


				length = sqrtf(powf(enemy[i].vec.x, 2) + powf(enemy[i].vec.y, 2));

				if (length != 0.0f)
				{
					enemy[i].vec.x = enemy[i].vec.x / length;
					enemy[i].vec.y = enemy[i].vec.y / length;
				}

				enemy[i].shape.translate.x += enemy[i].vel.x * enemy[i].vec.x;
				enemy[i].shape.translate.y += enemy[i].vel.y * enemy[i].vec.y;

				enemy[i].pos.world = VertexAffineMatrix(enemy[i].shape);

				break;
			}


			// プレイヤーの方向を向く
			if (enemy[i].shape.translate.x > player->shape.translate.x)
			{
				enemy[i].directionNo = DIRECTION_LEFT;
			} else if (enemy[i].shape.translate.x < player->shape.translate.x)
			{
				enemy[i].directionNo = DIRECTION_RIGHT;
			}


			/*   位置   */

			if (enemy[i].shape.translate.y - enemy[i].shape.scale.y < 0.0f)
			{
				enemy[i].shape.translate.y = enemy[i].shape.scale.y;
			}

			if (enemy[i].shape.translate.x - enemy[i].shape.scale.x < 0.0f)
			{
				enemy[i].shape.translate.x = enemy[i].shape.scale.x;
			}

			if (enemy[i].shape.translate.x + enemy[i].shape.scale.x > static_cast<float>(kScreenWtidh))
			{
				enemy[i].shape.translate.x = static_cast<float>(kScreenWtidh) - enemy[i].shape.scale.x;
			}
		}
	}
}