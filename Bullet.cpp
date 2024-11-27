#include "Structure.h"
#include "Constant.h"
#include "Enumeration.h"
#include "Shapes.h"
#include "Matrix.h"
#include "Bullet.h"
#include "Particle.h"

/// <summary>
/// 弾を発射する
/// </summary>
/// <param name="bullet">弾</param>
/// <param name="type">種類</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
void BulletShot(Bullet* bullet, int type, float posX, float posY)
{
	// null を探す
	if (bullet == nullptr)
	{
		return;
	}

	// 弾を発射する
	for (int i = 0; i < kBulletNum; i++)
	{
		if (bullet[i].isShot == false && bullet[i].respawn.isRespawn)
		{
			// 弾を発射する（発射フラグをtrueにする）
			bullet[i].isShot = true;

			// フレーム
			bullet[i].frame = 0;

			// 種類
			bullet[i].type = type;

			switch (bullet[i].type)
			{
			case BULLET_TYPE_EXPLOSION:

				// 図形
				bullet[i].shape.scale = { 120.0f , 120.0f };
				bullet[i].shape.theta = 0.0f;
				bullet[i].shape.translate = { posX , posY };

				// 移動速度
				bullet[i].vel = { 0.0f , 0.0f };

				break;

			case BULLET_TYPE_VIBRATION_LEFT:

				// 図形
				bullet[i].shape.scale = { 40.0f , 20.0f };
				bullet[i].shape.theta = 0.0f;
				bullet[i].shape.translate = { posX , posY };

				// 移動速度
				bullet[i].vel = { -8.0f , 0.0f };

				break;

			case BULLET_TYPE_VIBRATION_RIGHT:

				// 図形
				bullet[i].shape.scale = { 40.0f , 20.0f };
				bullet[i].shape.theta = 0.0f;
				bullet[i].shape.translate = { posX , posY };

				// 移動速度
				bullet[i].vel = { 8.0f , 0.0f };

				break;

			case BULLET_TYPE_RUBBLE:

				// 図形
				bullet[i].shape.scale = { 24.0f , 24.0f };
				bullet[i].shape.theta = 0.0f;
				bullet[i].shape.translate = { posX , posY };

				// 移動速度
				bullet[i].vel = { 0.0f , -3.0f };

				break;
			}

			// 位置
			bullet[i].pos.world = VertexAffineMatrix(bullet[i].shape);

			break;
		}
	}
}

/// <summary>
/// 弾を動かす
/// </summary>
/// <param name="bullet">弾</param>
void BulletMove(Bullet* bullet , Particle* particle)
{
	// null を探す
	if (bullet == nullptr || particle == nullptr)
	{
		return;
	}

	// 発射されている（発射フラグがtrueである）弾を動かす
	for (int i = 0; i < kBulletNum; i++)
	{
		if (bullet[i].isShot)
		{
			// 弾の時間を数える
			bullet[i].frame++;

			// 弾の種類で動きを変える
			switch (bullet[i].type)
			{
			case BULLET_TYPE_EXPLOSION:

				// 1フレームを超えたら、消える（復活、発射フラグがfalseになる）
				if (bullet[i].frame > 1)
				{
					bullet[i].respawn.isRespawn = false;
					bullet[i].isShot = false;
				}

				// 弾を動かす
				bullet[i].shape.translate.x += bullet[i].vel.x;
				bullet[i].shape.translate.y += bullet[i].vel.y;
				bullet[i].pos.world = VertexAffineMatrix(bullet[i].shape);

				break;

			case BULLET_TYPE_VIBRATION_LEFT:

				// 弾を動かす
				bullet[i].shape.translate.x += bullet[i].vel.x;
				bullet[i].shape.translate.y += bullet[i].vel.y;
				bullet[i].pos.world = VertexAffineMatrix(bullet[i].shape);

				if (bullet[i].frame % 5 == 0)
				{
					ParticleEmission(particle, PARTICLE_TYPE_SPARK_RIGHT, bullet[i].shape.translate.x - bullet[i].shape.scale.x, 0.0f);
				}

				// 画面外に出たら消える（発射フラグがtrueになる）
				if (bullet[i].shape.translate.x + bullet[i].shape.scale.x < 0.0f ||
					bullet[i].shape.translate.x - bullet[i].shape.scale.x > static_cast<float>(kScreenWtidh) ||
					bullet[i].shape.translate.y - bullet[i].shape.scale.y > static_cast<float>(kScreenHeight) - 120.0f ||
					bullet[i].shape.translate.y + bullet[i].shape.scale.y < -120.0f)
				{
					bullet[i].isShot = false;
				}

				break;

			case BULLET_TYPE_VIBRATION_RIGHT:

				// 弾を動かす
				bullet[i].shape.translate.x += bullet[i].vel.x;
				bullet[i].shape.translate.y += bullet[i].vel.y;
				bullet[i].pos.world = VertexAffineMatrix(bullet[i].shape);

				if (bullet[i].frame % 5 == 0)
				{
					ParticleEmission(particle, PARTICLE_TYPE_SPARK_LEFT, bullet[i].shape.translate.x + bullet[i].shape.scale.x, 0.0f);
				}

				// 画面外に出たら消える（発射フラグがtrueになる）
				if (bullet[i].shape.translate.x + bullet[i].shape.scale.x < 0.0f ||
					bullet[i].shape.translate.x - bullet[i].shape.scale.x > static_cast<float>(kScreenWtidh) ||
					bullet[i].shape.translate.y - bullet[i].shape.scale.y > static_cast<float>(kScreenHeight) - 120.0f ||
					bullet[i].shape.translate.y + bullet[i].shape.scale.y < -120.0f)
				{
					bullet[i].isShot = false;
				}

				break;

			case BULLET_TYPE_RUBBLE:

				// 弾を動かす
				bullet[i].shape.translate.x += bullet[i].vel.x;
				bullet[i].shape.translate.y += bullet[i].vel.y;
				bullet[i].pos.world = VertexAffineMatrix(bullet[i].shape);

				if (bullet[i].frame % 5 == 0)
				{
					ParticleEmission(particle, PARTICLE_TYPE_GRAVITY_INVERT, bullet[i].shape.translate.x, bullet[i].shape.translate.y);
				}

				// 画面外に出たら消える（発射フラグがtrueになる）
				if (bullet[i].shape.translate.x + bullet[i].shape.scale.x < 0.0f ||
					bullet[i].shape.translate.x - bullet[i].shape.scale.x > static_cast<float>(kScreenWtidh) ||
					bullet[i].shape.translate.y - bullet[i].shape.scale.y < 0.0f)
				{
					ParticleEmission(particle, PARTICLE_TYPE_RUBBLE_COLLISION, bullet[i].shape.translate.x, bullet[i].shape.translate.y);

					bullet[i].isShot = false;
				}

				break;
			}
		}
	}
}