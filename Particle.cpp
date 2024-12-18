#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Structure.h"
#include "Enumeration.h"
#include "Constant.h"
#include "Particle.h"
#include "Matrix.h"
#include "Shapes.h"

/// <summary>
/// パーティクルを放出する
/// </summary>
/// <param name="particle">パーティクル</param>
/// <param name="type">種類</param>
void ParticleEmission(Particle* particle , int type , float posX , float posY)
{
	// null を探す
	if (particle == nullptr)
	{
		return;
	}

	// 乱数
	unsigned int currentTimer = static_cast<unsigned int>(time(nullptr));
	srand(currentTimer);

	switch (type)
	{
	case PARTICLE_TYPE_COLLISION:

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_COLLISION;

					// 放出する時間
					particle[j].emitStartTimer = 60 + rand() % 60;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 3 + 2);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX , posY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[i].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[i].pos.world);

					// 放出方向
					float theta = static_cast<float>(rand() % 180);

					particle[j].vec = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

					// 移動速度
					float vel = static_cast<float>(rand() % 8 + 4);

					particle[j].vel = { vel , vel };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_AURA:

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_AURA;

					// 放出する時間
					particle[j].emitStartTimer = 60 + rand() % 60;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 4 + 8);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX , posY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[i].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[i].pos.world);

					// 放出方向
					float theta = static_cast<float>(rand() % 360);

					particle[j].vec = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

					// 移動速度
					float vel = static_cast<float>(rand() % 2 + 1);

					particle[j].vel = { vel , vel };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_CLOUD:

		for (int j = 0; j < kParticleNum; j++)
		{
			if (particle[j].isEmission == false)
			{
				// パーティクルを放出する（放出フラグがtrueになる）
				particle[j].isEmission = true;

				// 種類
				particle[j].type = PARTICLE_TYPE_CLOUD;

				// 放出する時間
				particle[j].emitStartTimer = 120;
				particle[j].emitTimer = particle[j].emitStartTimer;

				particle[j].shape.scale = { 4.0f , 4.0f };
				particle[j].shape.theta = 0.0f;
				particle[j].shape.translate = { posX , posY };

				// 位置
				particle[j].pos.world = VertexAffineMatrix(particle[j].shape);
				particle[j].pos.screen = CoordinateTransformation(particle[j].pos.world);

				// 放出方向
				particle[j].vec = { 0.0f , 0.0f };

				// 移動速度
				particle[j].vel = { 0.0f , 0.0f };

				break;
			}
		}

		break;

	case PARTICLE_TYPE_BOMB:

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_FIRE;

					// 放出する時間
					particle[j].emitStartTimer = 30 + rand() % 30;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float posRandX = static_cast<float>(rand() % 71 - 35);
					float posRandY = static_cast<float>(rand() % 71 - 35);

					particle[j].shape.scale = { 0.0f , 0.0f };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX + posRandX , posY + posRandY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[j].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[j].pos.world);

					particle[j].vec = { 0.0f , 0.0f};

					// 移動速度
					particle[j].vel = { 0.0f , 0.0f };

					for (int k = 0; k < 5; k++)
					{
						for (int l = 0; l < kParticleNum; l++)
						{
							if (particle[l].isEmission == false)
							{
								// パーティクルを放出する（放出フラグがtrueになる）
								particle[l].isEmission = true;

								// 種類
								particle[l].type = PARTICLE_TYPE_SMOKE_BLACK;

								// 放出する時間
								particle[l].emitStartTimer = 30 + rand() % 30;
								particle[l].emitTimer = particle[l].emitStartTimer;

								// 図形
								posRandX = static_cast<float>(rand() % 71 - 35);
								posRandY = static_cast<float>(rand() % 71 - 35);

								particle[l].shape.scale = { 0.0f , 0.0f };
								particle[l].shape.theta = 0.0f;
								particle[l].shape.translate = { particle[j].shape.translate.x +  posRandX , particle[j].shape.translate.y + posRandY };

								// 位置
								particle[l].pos.world = VertexAffineMatrix(particle[l].shape);
								particle[l].pos.screen = CoordinateTransformation(particle[l].pos.world);

								particle[l].vec = { 0.0f , 0.0f };

								// 移動速度
								particle[l].vel = { 0.0f , 0.0f };

								break;
							}
						}
					}


					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_RUBBLE_GRAVITY:

		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類!
					particle[j].type = PARTICLE_TYPE_RUBBLE_GRAVITY;

					// 放出する時間
					particle[j].emitStartTimer = 30 + rand() % 70;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 6 + 2);

					float randPosX = static_cast<float>(rand() % kScreenWtidh);
					float randPosY = static_cast<float>(kScreenHeight - 50 + rand() % 100);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { randPosX , randPosY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[i].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[i].pos.world);

					// 放出方向
					particle[j].vec = { 0.0f , 1.0f };

					// 移動速度
					particle[j].vel = { 0.0f , 0.0f };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_SPARK_LEFT:

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_SPARK;

					// 放出する時間
					particle[j].emitStartTimer = 10 + rand() % 20;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 3 + 2);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX , posY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[j].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[j].pos.world);

					// 放出方向
					float theta = static_cast<float>(110 + rand() % 70);

					particle[j].vec = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

					// 移動速度
					float vel = static_cast<float>(rand() % 4 + 1);

					particle[j].vel = { vel , vel * 2 };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_SPARK_RIGHT:

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_SPARK;

					// 放出する時間
					particle[j].emitStartTimer = 10 + rand() % 20;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 3 + 2);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX , posY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[j].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[j].pos.world);

					// 放出方向
					float theta = static_cast<float>(10 + rand() % 70);

					particle[j].vec = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

					// 移動速度
					float vel = static_cast<float>(rand() % 4 + 1);

					particle[j].vel = { vel , vel * 2 };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_SUMMON:

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_SUMMON;

					// 放出する時間
					particle[j].emitStartTimer = 180 + rand() % 60;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float posRandX = static_cast<float>(rand() % 25 - 12);
					float posRandY = static_cast<float>(rand() % 25 - 12);

					float theta = static_cast<float>(rand() % 360);

					particle[j].shape.scale = { 0.0f , 0.0f };
					particle[j].shape.theta = theta;
					particle[j].shape.translate = { posX + posRandX , posY + posRandY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[j].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[j].pos.world);

					particle[j].vec = { 0.0f , 0.0f };

					// 移動速度
					particle[j].vel = { 0.0f , 0.0f };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_GRAVITY:

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_GRAVITY;

					// 放出する時間
					particle[j].emitStartTimer = 30 + rand() % 60;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 3 + 1);

					float randPosX = static_cast<float>(rand() % 31 - 15);
					float randPosY = static_cast<float>(rand() % 31 - 15);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX + randPosX , posY + randPosY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[i].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[i].pos.world);

					// 放出方向
					particle[j].vec = { 0.0f , 1.0f };

					// 移動速度
					particle[j].vel = { 0.0f , 0.0f };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_GRAVITY_INVERT:

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_GRAVITY_INVERT;

					// 放出する時間
					particle[j].emitStartTimer = 30 + rand() % 60;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float randPosX = static_cast<float>(rand() % 41 - 20);
					float randPosY = static_cast<float>(rand() % 41 - 20);

					particle[j].shape.scale = { 0.0f , 0.0f };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX + randPosX , posY + randPosY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[i].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[i].pos.world);

					// 放出方向
					particle[j].vec = { 0.0f , 1.0f };

					// 移動速度
					particle[j].vel = { 0.0f , 1.0f };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_RUBBLE_COLLISION:

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_COLLISION;

					// 放出する時間
					particle[j].emitStartTimer = 60 + rand() % 60;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 6 + 10);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX , posY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[i].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[i].pos.world);

					// 放出方向
					float theta = static_cast<float>(rand() % 180);

					particle[j].vec = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

					// 移動速度
					float vel = static_cast<float>(rand() % 4 + 1);

					particle[j].vel = { vel , vel };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_WALK:

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_WALK;

					// 放出する時間
					particle[j].emitStartTimer = 10 + rand() % 30;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float posRandX = static_cast<float>(rand() % 11 - 5);
					float posRandY = static_cast<float>(rand() % 11 - 5);

					particle[j].shape.scale = { 0.0f , 0.0f };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX + posRandX , posY + posRandY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[j].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[j].pos.world);

					particle[j].vec = { 0.0f , 0.0f };

					// 移動速度
					particle[j].vel = { 0.0f , 0.0f };

					break;
				}
			}
		}

		break;

	case PARTICLE_TYPE_RUSH:

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_RUSH;

					// 放出する時間
					particle[j].emitStartTimer = 10 + rand() % 30;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float size = static_cast<float>(rand() % 3 + 2);

					particle[j].shape.scale = { size , size };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX , posY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[i].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[i].pos.world);

					// 放出方向
					float theta = static_cast<float>(rand() % 180 + 90);

					particle[j].vec = { cosf((theta / 180.0f) * float(M_PI)) , sinf((theta / 180.0f) * float(M_PI)) };

					// 移動速度

					particle[j].vel = { 12.0f , 12.0f };

					break;
				}
			}
		}

		break;


	case PARTICLE_TYPE_BIG_BOMB:

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < kParticleNum; j++)
			{
				if (particle[j].isEmission == false)
				{
					// パーティクルを放出する（放出フラグがtrueになる）
					particle[j].isEmission = true;

					// 種類
					particle[j].type = PARTICLE_TYPE_BIG_FIRE;

					// 放出する時間
					particle[j].emitStartTimer = 30 + rand() % 30;
					particle[j].emitTimer = particle[j].emitStartTimer;

					// 図形
					float posRandX = static_cast<float>(rand() % 71 - 35);
					float posRandY = static_cast<float>(rand() % 71 - 35);

					particle[j].shape.scale = { 0.0f , 0.0f };
					particle[j].shape.theta = 0.0f;
					particle[j].shape.translate = { posX + posRandX , posY + posRandY };

					// 位置
					particle[j].pos.world = VertexAffineMatrix(particle[j].shape);
					particle[j].pos.screen = CoordinateTransformation(particle[j].pos.world);

					particle[j].vec = { 0.0f , 0.0f };

					// 移動速度
					particle[j].vel = { 0.0f , 0.0f };

					for (int k = 0; k < 5; k++)
					{
						for (int l = 0; l < kParticleNum; l++)
						{
							if (particle[l].isEmission == false)
							{
								// パーティクルを放出する（放出フラグがtrueになる）
								particle[l].isEmission = true;

								// 種類
								particle[l].type = PARTICLE_TYPE_BIG_SMOKE_BLACK;

								// 放出する時間
								particle[l].emitStartTimer = 30 + rand() % 30;
								particle[l].emitTimer = particle[l].emitStartTimer;

								// 図形
								posRandX = static_cast<float>(rand() % 71 - 35);
								posRandY = static_cast<float>(rand() % 71 - 35);

								particle[l].shape.scale = { 0.0f , 0.0f };
								particle[l].shape.theta = 0.0f;
								particle[l].shape.translate = { particle[j].shape.translate.x + posRandX , particle[j].shape.translate.y + posRandY };

								// 位置
								particle[l].pos.world = VertexAffineMatrix(particle[l].shape);
								particle[l].pos.screen = CoordinateTransformation(particle[l].pos.world);

								particle[l].vec = { 0.0f , 0.0f };

								// 移動速度
								particle[l].vel = { 0.0f , 0.0f };

								break;
							}
						}
					}


					break;
				}
			}
		}

		break;
	}
}

/// <summary>
/// パーティクルを動かす
/// </summary>
/// <param name="particle"></param>
void PartivleMove(Particle* particle)
{
	// null を探す
	if (particle == nullptr)
	{
		return;
	}

	// 放出された（放出フラグがtrueである）パーティクルを動かす
	for (int i = 0; i < kParticleNum; i++)
	{
		if (particle[i].isEmission)
		{
			particle[i].emitTimer--;

			float frameRate = 0.0f;
			float easedT = 0.0f;

			switch (particle[i].type)
			{
			case PARTICLE_TYPE_COLLISION:

				particle[i].vel.y += -0.5f;

				break;

			case PARTICLE_TYPE_CLOUD:

				frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

				easedT = 1.0f - powf(1.0f - frameRate, 4);

				particle[i].shape.scale.x = easedT * 4.0f + (1.0f - easedT) * 0.0f;
				particle[i].shape.scale.y = easedT * 4.0f + (1.0f - easedT) * 0.0f;

				break;

			case PARTICLE_TYPE_FIRE:

				if (particle[i].emitTimer >= particle[i].emitStartTimer - particle[i].emitStartTimer / 24)
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

					easedT = 1.0f - powf(1.0f - frameRate, 4);

					particle[i].shape.scale.x = easedT * 96.0f + (1.0f - easedT) * 0.0f;
					particle[i].shape.scale.y = easedT * 96.0f + (1.0f - easedT) * 0.0f;
				}
				else
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer - particle[i].emitStartTimer / 24));

					easedT = powf(frameRate, 4);

					particle[i].shape.scale.x = easedT * 0.0f + (1.0f - easedT) * 96.0f;
					particle[i].shape.scale.y = easedT * 0.0f + (1.0f - easedT) * 96.0f;
				}

				break;

			case PARTICLE_TYPE_SMOKE_BLACK:

				if (particle[i].emitTimer >= particle[i].emitStartTimer - particle[i].emitStartTimer / 24)
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

					easedT = 1.0f - powf(1.0f - frameRate, 4);

					particle[i].shape.scale.x = easedT * 32.0f + (1.0f - easedT) * 0.0f;
					particle[i].shape.scale.y = easedT * 32.0f + (1.0f - easedT) * 0.0f;
				} else
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer - particle[i].emitStartTimer / 24));

					easedT = powf(frameRate, 4);

					particle[i].shape.scale.x = easedT * 0.0f + (1.0f - easedT) * 32.0f;
					particle[i].shape.scale.y = easedT * 0.0f + (1.0f - easedT) * 32.0f;
				}

				break;

			case PARTICLE_TYPE_RUBBLE_GRAVITY:

				particle[i].vel.y += -0.1f;

				break;

			case PARTICLE_TYPE_SPARK:

				particle[i].vel.y += -0.5f;

				break;

			case PARTICLE_TYPE_SUMMON:

				if (particle[i].emitTimer >= particle[i].emitStartTimer - particle[i].emitStartTimer / 3)
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

					easedT = 1.0f - powf(1.0f - frameRate, 4);

					particle[i].shape.scale.x = easedT * 32.0f + (1.0f - easedT) * 0.0f;
					particle[i].shape.scale.y = easedT * 32.0f + (1.0f - easedT) * 0.0f;
				} 
				else
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer - particle[i].emitStartTimer / 3));

					easedT = powf(frameRate, 4);

					particle[i].shape.scale.x = easedT * 0.0f + (1.0f - easedT) * 32.0f;
					particle[i].shape.scale.y = easedT * 0.0f + (1.0f - easedT) * 32.0f;
				}

				particle[i].shape.theta += 1.0f;

				break;

			case PARTICLE_TYPE_GRAVITY:

				particle[i].vel.y += -0.05f;

				break;

			case PARTICLE_TYPE_GRAVITY_INVERT:

				if (particle[i].emitTimer >= particle[i].emitStartTimer - particle[i].emitStartTimer / 15)
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

					easedT = 1.0f - powf(1.0f - frameRate, 4);

					particle[i].shape.scale.x = easedT * 2.0f + (1.0f - easedT) * 0.0f;
					particle[i].shape.scale.y = easedT * 2.0f + (1.0f - easedT) * 0.0f;
				} 
				else
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer - particle[i].emitStartTimer / 15));

					easedT = powf(frameRate, 4);

					particle[i].shape.scale.x = easedT * 0.0f + (1.0f - easedT) * 2.0f;
					particle[i].shape.scale.y = easedT * 0.0f + (1.0f - easedT) * 2.0f;
				}

				break;

			case PARTICLE_TYPE_WALK:

				if (particle[i].emitTimer >= particle[i].emitStartTimer - particle[i].emitStartTimer / 20)
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

					easedT = 1.0f - powf(1.0f - frameRate, 4);

					particle[i].shape.scale.x = easedT * 4.0f + (1.0f - easedT) * 0.0f;
					particle[i].shape.scale.y = easedT * 4.0f + (1.0f - easedT) * 0.0f;
				} else
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer - particle[i].emitStartTimer / 20));

					easedT = powf(frameRate, 4);

					particle[i].shape.scale.x = easedT * 0.0f + (1.0f - easedT) * 4.0f;
					particle[i].shape.scale.y = easedT * 0.0f + (1.0f - easedT) * 4.0f;
				}

				break;

			case PARTICLE_TYPE_RUSH:

				particle[i].vel.x += -0.1f;

				break;

			case PARTICLE_TYPE_BIG_FIRE:

				if (particle[i].emitTimer >= particle[i].emitStartTimer - particle[i].emitStartTimer / 24)
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

					easedT = 1.0f - powf(1.0f - frameRate, 4);

					particle[i].shape.scale.x = easedT * 192.0f + (1.0f - easedT) * 0.0f;
					particle[i].shape.scale.y = easedT * 288.0f + (1.0f - easedT) * 0.0f;
				} else
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer - particle[i].emitStartTimer / 24));

					easedT = powf(frameRate, 4);

					particle[i].shape.scale.x = easedT * 0.0f + (1.0f - easedT) * 192.0f;
					particle[i].shape.scale.y = easedT * 0.0f + (1.0f - easedT) * 192.0f;
				}

				break;

			case PARTICLE_TYPE_BIG_SMOKE_BLACK:

				if (particle[i].emitTimer >= particle[i].emitStartTimer - particle[i].emitStartTimer / 24)
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer));

					easedT = 1.0f - powf(1.0f - frameRate, 4);

					particle[i].shape.scale.x = easedT * 64.0f + (1.0f - easedT) * 0.0f;
					particle[i].shape.scale.y = easedT * 64.0f + (1.0f - easedT) * 0.0f;
				} else
				{
					frameRate = 1.0f - (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer - particle[i].emitStartTimer / 24));

					easedT = powf(frameRate, 4);

					particle[i].shape.scale.x = easedT * 0.0f + (1.0f - easedT) * 64.0f;
					particle[i].shape.scale.y = easedT * 0.0f + (1.0f - easedT) * 64.0f;
				}

				break;
			}

			particle[i].shape.translate.x += particle[i].vel.x * particle[i].vec.x;
			particle[i].shape.translate.y += particle[i].vel.y * particle[i].vec.y;
			particle[i].pos.world = VertexAffineMatrix(particle[i].shape);

			if (particle[i].emitTimer <= 0)
			{
				particle[i].isEmission = false;
			}
		}
	}
}