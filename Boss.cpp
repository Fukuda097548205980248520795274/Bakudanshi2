#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Structure.h"
#include "Constant.h"
#include "Enumeration.h"
#include "Shapes.h"
#include "Matrix.h"
#include "Process.h"
#include "Player.h"
#include "Bomb.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Boss.h"
#include "Particle.h"
#include "Novice.h"

/// <summary>
/// ボスを出現させる!
/// </summary>
/// <param name="boss">ボス</param>
/// <param name="type">種類</param>
void BossArrival(Boss* boss, int type)
{
	// null を探す
	if (boss == nullptr)
	{
		return;
	}

	if (boss->isArrival == false && boss->respawn.isRespawn)
	{
		// ボスを出現させる（出現フラグをtrueにする）
		boss->isArrival = true;

		// フレーム
		boss->frame = 0;

		// 種類
		boss->type = type;

		// 向いている方向
		boss->directionNo = DIRECTION_LEFT;

		// 攻撃させない（攻撃フラグをfalseにする）!
		boss->isAttack = false;
		boss->attackNo = -1;

		switch (boss->type)
		{
		case BOSS_TYPE_STAGE_1:

			// 図形
			boss->shape.scale = { 32.0f , 64.0f };
			boss->shape.theta = 0.0f;
			boss->shape.translate = { 980.0f , boss->shape.scale.y };

			// 体力
			boss->damage.hp = 1000;
			boss->damage.isDamage = false;
			boss->damage.timer = 30;

			break;
		}

		// 位置
		boss->pos.world = VertexAffineMatrix(boss->shape);
		boss->pos.screen = CoordinateTransformation(boss->pos.world);

		// 移動速度
		boss->vel = { 0.0f , 0.0f };

		// ジャンプ
		boss->jump.isJump = false;
		boss->jump.count = 0;
	}
}

/// <summary>
/// ボスを動かす
/// </summary>
/// <param name="boss">ボス</param>
/// <param name="player">プレイヤー</param>
/// <param name="bullet">弾</param>
/// <param name="enemy">敵</param>
void BossMove(Boss* boss, Player* player, Bullet* bullet, Enemy* enemy, Particle* particle , BackGround* backGround, SH* sh)
{
	// null を探す
	if (boss == nullptr || player == nullptr || bullet == nullptr || enemy == nullptr || particle == nullptr)
	{
		return;
	}

	// 乱数
	unsigned int currentTimer = static_cast<unsigned int>(time(nullptr));
	srand(currentTimer);

	// 出現している（出現フラグがtrueである）ボスを動かす
	if (boss->isArrival)
	{
		// フレームを進める
		boss->frame++;

		// 横移動を初期化する
		boss->vel.x = 0.0f;

		// 20フレームから、攻撃を始める
		if (boss->frame >= 20)
		{
			switch (boss->type)
			{
			case BOSS_TYPE_STAGE_1:

				// 攻撃していない（攻撃フラグがfalseである）ときは、攻撃をランダムに決める
				if (boss->isAttack == false)
				{
					if (boss->damage.hp > 750)
					{
						if (rand() % 2 == 0)
						{
							boss->attackNo = 0;
						} 
						else
						{
							boss->attackNo = 1;
						}
					} 
					else if (boss->damage.hp > 500)
					{
						int randam = rand() % 3;

						if (randam == 0)
						{
							boss->attackNo = 0;
						}
						else if (randam == 1)
						{
							boss->attackNo = 4;
						}
						else
						{
							boss->attackNo = 3;
						}
					} 
					else if (boss->damage.hp > 250)
					{
						int randam = rand() % 3;

						if (randam == 0)
						{
							boss->attackNo = 1;
						}
						else if (randam == 1)
						{
							boss->attackNo = 0;
						}
						else
						{
							boss->attackNo = 3;
						}
					} 
					else
					{
						int randam = rand() % 4;

						if (randam == 0)
						{
							boss->attackNo = 0;
						} 
						else if (randam == 1)
						{
							boss->attackNo = 1;
						} 
						else if (randam == 2)
						{
							boss->attackNo = 3;
						} 
						else
						{
							boss->attackNo = 2;
						}
					}

					// 攻撃を行う（攻撃フラグがtrueになる）
					boss->isAttack = true;
				} 
				else
				{

					switch (boss->attackNo)
					{
					case 0:

						/*   突進   */

						// プレイヤーのいる方向に突進する
						if (boss->frame == 55)
						{
							if (boss->shape.translate.x - boss->shape.scale.x > 0.0f &&
								boss->shape.translate.x + boss->shape.scale.x < static_cast<float>(kScreenWtidh))
							{
								boss->frame--;

								if (boss->directionNo == DIRECTION_LEFT)
								{
									boss->vel.x = -14.0f;

								} 
								else if (boss->directionNo == DIRECTION_RIGHT)
								{
									boss->vel.x = 14.0f;
								}
							}
							else
							{
								ParticleEmission(particle, PARTICLE_TYPE_RUBBLE_GRAVITY, 0.0f, 0.0f);

								if (boss->directionNo == DIRECTION_LEFT)
								{
									ParticleEmission(particle, PARTICLE_TYPE_SPARK_RIGHT, boss->shape.translate.x - boss->shape.scale.x, boss->shape.translate.y);
								}
								else if (boss->directionNo == DIRECTION_RIGHT)
								{
									ParticleEmission(particle, PARTICLE_TYPE_SPARK_LEFT, boss->shape.translate.x + boss->shape.scale.x, boss->shape.translate.y);
								}
							}
							if (!Novice::IsPlayingAudio(sh->pHBossDash) || sh->pHBossDash == -1) {
								sh->pHBossDash = Novice::PlayAudio(sh->bossDash, 0, 0.6f);
							}

						}

						// 跳ね飛ぶ
						if (boss->frame == 56)
						{
							boss->frame--;

							if (boss->directionNo == DIRECTION_LEFT)
							{
								boss->vel.x = static_cast<float>(4 - boss->jump.count) * 2.0f;
							} else if (boss->directionNo == DIRECTION_RIGHT)
							{
								boss->vel.x = static_cast<float>(4 - boss->jump.count) * -2.0f;
							}

							if (boss->jump.isJump == false)
							{
								if (boss->jump.count >= 3)
								{
									boss->frame++;
								}

								if (boss->jump.count < 3)
								{
									boss->jump.isJump = true;
									boss->vel.y = 6.0f;
									boss->jump.fallingVel = 1.0f;

									boss->jump.count++;
								}
							}

							if (!Novice::IsPlayingAudio(sh->pHHitWall) || sh->pHHitWall == -1) {
								sh->pHHitWall = Novice::PlayAudio(sh->hitWall, 0, 0.9f);
							}

							// シェイク
							if (!backGround->isShake) {
								backGround->isShake = true;
								backGround->shakeRange.x = boss->shakeStrong.x;
								backGround->shakeRange.y = boss->shakeWeak.y;
								backGround->randSpeed.x = boss->shakeStrong.x / boss->shakeFrame;
								backGround->randSpeed.y = boss->shakeWeak.y / boss->shakeFrame;
							}
						}

						// 攻撃終了
						if (boss->frame == 130)
						{
							boss->isAttack = false;
							boss->attackNo = -1;

							boss->frame = -20;

							boss->jump.count = 0;
						}

						break;

					case 1:

						/*   振動   */

						if (boss->frame >= 60 && boss->frame <= 160)
						{
							if (boss->directionNo == DIRECTION_LEFT)
							{
								boss->vel.x = -4.0f;
							} else if (boss->directionNo == DIRECTION_RIGHT)
							{
								boss->vel.x = 4.0f;
							}

							if (boss->frame == 60) {
								Novice::PlayAudio(sh->bossJump, 0, 0.6f);
							}
						}

						if (boss->frame >= 60 && boss->frame <= 120)
						{
							int startFrame = boss->frame - 60;
							int endFrame = 60;

							float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);
							float easedT = 1.0f - powf(1.0f - frameRate, 2);

							boss->shape.translate.y = (1.0f - easedT) * boss->shape.scale.y + easedT * 400.0f;
						}

						if (boss->frame >= 120 && boss->frame <= 160)
						{
							int startFrame = boss->frame - 120;
							int endFrame = 40;

							float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);
							float easedT = powf(frameRate, 3);

							boss->shape.translate.y = (1.0f - easedT) * 400.0f + easedT * boss->shape.scale.y;

							if (boss->frame % 5 == 0)
							{
								ParticleEmission(particle, PARTICLE_TYPE_GRAVITY_INVERT, boss->shape.translate.x, boss->shape.translate.y);
							}
						}

						if (boss->frame == 160)
						{
							// シェイク
							if (!backGround->isShake) {
								backGround->isShake = true;
								backGround->shakeRange.x = boss->shakeWeak.x;
								backGround->shakeRange.y = boss->shakeStrong.y;
								backGround->randSpeed.x = boss->shakeWeak.x / boss->shakeFrame;
								backGround->randSpeed.y = boss->shakeStrong.y / boss->shakeFrame;
							}

							if (!Novice::IsPlayingAudio(sh->pHHitWall) || sh->pHHitWall == -1) {
								sh->pHHitWall = Novice::PlayAudio(sh->hitWall, 0, 0.9f);
							}

							if (!Novice::IsPlayingAudio(sh->pHEarthqueke) || sh->pHEarthqueke == -1) {
								sh->pHEarthqueke = Novice::PlayAudio(sh->earthqueke, 0, 1.0f);
							}
							// パーティクル
							ParticleEmission(particle, PARTICLE_TYPE_COLLISION, boss->shape.translate.x, boss->shape.translate.y - boss->shape.scale.y);
							ParticleEmission(particle, PARTICLE_TYPE_RUBBLE_GRAVITY, 0.0f, 0.0f);
						}

						if (boss->frame >= 160 && boss->frame <= 280)
						{
							if ((boss->frame - 160) % 60 == 0)
							{
								BulletShot(bullet, BULLET_TYPE_VIBRATION_LEFT, boss->shape.translate.x, boss->shape.translate.y - boss->shape.scale.y);
								BulletShot(bullet, BULLET_TYPE_VIBRATION_RIGHT, boss->shape.translate.x, boss->shape.translate.y - boss->shape.scale.y);
							}
						}

						if (boss->frame >= 350)
						{
							boss->isAttack = false;
							boss->attackNo = -1;
							Novice::StopAudio(sh->pHEarthqueke);

							boss->frame = -40;
						}


						break;

					case 2:

						/*   がれき落とし   */

						if (boss->frame >= 80 && boss->frame <= 100)
						{
							int startFrame = boss->frame - 80;
							int endFrame = 20;

							float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

							boss->shape.translate.y = (1.0f - frameRate) * boss->shape.scale.y + frameRate * (670.0f - boss->shape.scale.y);

							ParticleEmission(particle , PARTICLE_TYPE_GRAVITY , boss->shape.translate.x , boss->shape.translate.y - boss->shape.scale.y);
						}

						if (boss->frame == 100)
						{
							// シェイク
							if (!backGround->isShake) {
								backGround->isShake = true;
								backGround->shakeRange.x = boss->shakeWeak.x;
								backGround->shakeRange.y = boss->shakeWeak.y;
								backGround->randSpeed.x = boss->shakeWeak.x / boss->shakeFrame;
								backGround->randSpeed.y = boss->shakeWeak.y / boss->shakeFrame;

								if (!Novice::IsPlayingAudio(sh->pHfallingRock) || sh->pHfallingRock == -1) {
									sh->pHfallingRock = Novice::PlayAudio(sh->fallingRock, 0, 1.0f);
								}
							}

							ParticleEmission(particle, PARTICLE_TYPE_RUBBLE_GRAVITY, 0.0f, 0.0f);

							if (!Novice::IsPlayingAudio(sh->pHHitWall) || sh->pHHitWall == -1) {
								sh->pHHitWall = Novice::PlayAudio(sh->hitWall, 0, 0.6f);
							}
						}


						if (boss->frame == 120)
						{
							for (int i = 0; i < 6; i++)
							{
								switch (i)
								{
								case 0:

									BulletShot(bullet, BULLET_TYPE_RUBBLE, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 1000.0f);
									ParticleEmission(particle , PARTICLE_TYPE_GRAVITY , static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 700.0f);

									break;

								case 1:

									BulletShot(bullet, BULLET_TYPE_RUBBLE, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 650.0f);
									ParticleEmission(particle, PARTICLE_TYPE_GRAVITY, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 700.0f);

									break;

								case 2:

									BulletShot(bullet, BULLET_TYPE_RUBBLE, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 1200.0f);
									ParticleEmission(particle, PARTICLE_TYPE_GRAVITY, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 700.0f);

									break;

								case 3:

									BulletShot(bullet, BULLET_TYPE_RUBBLE, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 700.0f);
									ParticleEmission(particle, PARTICLE_TYPE_GRAVITY, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 700.0f);

									break;

								case 4:

									BulletShot(bullet, BULLET_TYPE_RUBBLE, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 950.0f);
									ParticleEmission(particle, PARTICLE_TYPE_GRAVITY, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 700.0f);

									break;

								case 5:

									BulletShot(bullet, BULLET_TYPE_RUBBLE, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 800.0f);
									ParticleEmission(particle, PARTICLE_TYPE_GRAVITY, static_cast<float>(kScreenWtidh / 6) + static_cast<float>(kScreenWtidh / 7) * i, 700.0f);

									break;
								}
							}
						}


						if (boss->frame >= 150 && boss->frame <= 390)
						{
							int startFrame = boss->frame - 150;
							int endFrame = 240;

							float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);
							float easedT = 1.0f - powf(1.0f - frameRate, 2);

							boss->shape.translate.y = (1.0f - easedT) * (670.0f - boss->shape.scale.y) + easedT * boss->shape.scale.y;
						}

						// 攻撃終了
						if (boss->frame >= 400)
						{
							boss->isAttack = false;
							boss->attackNo = -1;

							Novice::StopAudio(sh->pHfallingRock);

							boss->frame = -40;
						}


						break;

					case 3:

						// 敵が6体いたら、突進攻撃に切り替わる
						if (boss->frame == 21)
						{
							int enemyCount = 0;

							for (int i = 0; i < kEnemyNum; i++)
							{
								if (enemy[i].isArrival)
								{
									enemyCount++;
								}
							}

							if (enemyCount > 3)
							{
								boss->attackNo = 0;

								boss->frame = 19;
							}
						}

						if (boss->frame == 50)
						{
							if (boss->directionNo == DIRECTION_LEFT)
							{
								for (int i = 0; i < 3; i++)
								{
									EnemyArrival(enemy, ENEMY_TYPE_GROUND, boss->shape.translate.x - 50.0f - i * 50.0f, 0.0f, sh);
								}
							} else if (boss->directionNo == DIRECTION_RIGHT)
							{
								for (int i = 0; i < 3; i++)
								{
									EnemyArrival(enemy, ENEMY_TYPE_GROUND, boss->shape.translate.x + 50.0f + i * 50.0f, 0.0f, sh);
								}
							}
						}

						// 攻撃終了
						if (boss->frame >= 120)
						{
							boss->isAttack = false;
							boss->attackNo = -1;

							Novice::StopAudio(sh->pHEnemyStart);

							boss->frame = -40;
						}


						break;

					case 4:

						// 敵が3体いたら、突進攻撃に切り替わる
						if (boss->frame == 21)
						{
							int enemyCount = 0;

							for (int i = 0; i < kEnemyNum; i++)
							{
								if (enemy[i].isArrival)
								{
									enemyCount++;
								}
							}

							if (enemyCount > 3)
							{
								boss->attackNo = 0;

								boss->frame = 19;
							}
						}

						if (boss->frame >= 30 && boss->frame <= 80)
						{
							int startFrame = boss->frame - 30;
							int endFrame = 50;

							float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);
							float easedT = 1.0f - powf(1.0f - frameRate, 2);

							boss->shape.translate.y = (1.0f - easedT) * boss->shape.scale.y + easedT * 300.0f;
						}

						if (boss->frame == 80)
						{
							ParticleEmission(particle, PARTICLE_TYPE_SUMMON, boss->shape.translate.x, boss->shape.translate.y + boss->shape.scale.y + 60.0f);
						}

						if (boss->frame >= 90 && boss->frame <= 190)
						{
							if ((boss->frame - 190) % 50 == 0)
							{
								EnemyArrival(enemy, ENEMY_TYPE_FLYING, boss->shape.translate.x, boss->shape.translate.y + boss->shape.scale.y + 60.0f, sh);
							}
						}

						if (boss->frame >= 240 && boss->frame <= 300)
						{
							int startFrame = boss->frame - 240;
							int endFrame = 60;

							float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);
							float easedT = 1.0f - powf(1.0f - frameRate, 2);

							boss->shape.translate.y = (1.0f - easedT) * 300.0f + easedT * boss->shape.scale.y;
						}

						if (boss->frame >= 330)
						{
							boss->isAttack = false;
							boss->attackNo = -1;

							boss->frame = -40;
						}

						break;
					}
				}

				break;
			}
		} else
		{
			// 20f以前は、プレイヤーの方向に近づく
			if (boss->shape.translate.x - 200.0f > player->shape.translate.x)
			{
				boss->vel.x = -1.0f;
			} else if (boss->shape.translate.x + 200.0f < player->shape.translate.x)
			{
				boss->vel.x = 1.0f;
			}

			// プレイヤーの方向を向く
			if (boss->shape.translate.x > player->shape.translate.x)
			{
				boss->directionNo = DIRECTION_LEFT;
			} else if (boss->shape.translate.x < player->shape.translate.x)
			{
				boss->directionNo = DIRECTION_RIGHT;
			}
		}

		if (boss->jump.isJump)
		{
			boss->vel.y -= boss->jump.fallingVel;

			boss->shape.translate.y += boss->vel.y;
			boss->pos.world = VertexAffineMatrix(boss->shape);

			if (boss->shape.translate.y - boss->shape.scale.y <= 0.0f)
			{
				boss->jump.isJump = false;
				boss->vel.y = 0.0f;
				boss->jump.fallingVel = 0.0f;
			}
		}

		boss->shape.translate.x += boss->vel.x;
		boss->pos.world = VertexAffineMatrix(boss->shape);
	}


	/*   位置   */

	// 地面
	if (boss->shape.translate.y - boss->shape.scale.y < 0.0f)
	{
		boss->shape.translate.y = boss->shape.scale.y;
		boss->pos.world = VertexAffineMatrix(boss->shape);
	}

	// 天井
	if (boss->shape.translate.y + boss->shape.scale.y > static_cast<float>(kScreenHeight) - 50.0f)
	{
		boss->shape.translate.y = static_cast<float>(kScreenHeight) - 50.0f - boss->shape.scale.y;
		boss->pos.world = VertexAffineMatrix(boss->shape);
	}

	// 左の壁
	if (boss->shape.translate.x - boss->shape.scale.x < 0.0f)
	{
		boss->shape.translate.x = boss->shape.scale.x;
		boss->pos.world = VertexAffineMatrix(boss->shape);
	}

	// 右の壁
	if (boss->shape.translate.x + boss->shape.scale.x > static_cast<float>(kScreenWtidh))
	{
		boss->shape.translate.x = static_cast<float>(kScreenWtidh) - boss->shape.scale.x;
		boss->pos.world = VertexAffineMatrix(boss->shape);
	}
}