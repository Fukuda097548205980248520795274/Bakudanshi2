#include "Structure.h"
#include "Process.h"
#include "Shapes.h"
#include "Novice.h"

/// <summary>
/// 復活処理を行う
/// </summary>
/// <param name="respawn">復活</param>
void RespawnProcess(Respawn* respawn)
{
	// nullを探す
	if (respawn == nullptr)
	{
		return;
	}

	// 復活していない（復活フラグがfalseである）ときに、復活処理を行う
	if (respawn->isRespawn == false)
	{

		respawn->timer--;

		if (respawn->timer <= 0)
		{
			// 復活する（復活フラグがtrueになる）
			respawn->isRespawn = true;

			// 復活処理を初期化する
			respawn->timer = 120;
		}
	}
}

/// <summary>
/// ダメージ処理を行う
/// </summary>
/// <param name="player"></param>
void DamageProcess(Player* player, SH* sh)
{
	// nullを探す
	if (player == nullptr || sh == nullptr)
	{
		return;
	}

	// ダメージが入っている（ダメージフラグがtrueである）ときに、ダメージ処理を行う
	if (player->damage.isDamage)
	{
		// 体力があるなら、ダメージ処理を行う
		if (player->damage.hp >= 0)
		{
			player->damage.timer--;

			if (player->damage.timer <= 0)
			{
				// ダメージが消える（ダメージフラグがfalseになる）
				player->damage.isDamage = false;

				// ダメージ処理を初期化する
				player->damage.timer = 30;
			}
		} else
		{
			// 体力が0未満だったらやられる（復活フラグがfalseになる）
			player->respawn.isRespawn = false;

			Novice::PlayAudio(sh->playerKO, 0, 1.0f);

			// ダメージが消える（ダメージフラグがfalseになる）
			player->damage.isDamage = false;

			// ダメージ処理を初期化する
			player->damage.timer = 30;
		}
	}
}

/// <summary>
/// ダメージ処理を行う
/// </summary>
/// <param name="enemy">敵</param>
void DamageProcess(Enemy* enemy)
{
	// null を探す
	if (enemy == nullptr)
	{
		return;
	}

	// ダメージを受けている（ダメージフラグがtrueである）ときに、ダメージ処理を行う
	if (enemy->damage.isDamage)
	{
		// 体力があるなら、ダメージ処理を行う
		if (enemy->damage.hp >= 0)
		{
			enemy->damage.timer--;

			if (enemy->damage.timer <= 0)
			{
				// ダメージが消える（ダメージフラグがfalseになる）
				enemy->damage.isDamage = false;

				// ダメージ処理を初期化する
				enemy->damage.timer = 30;
			}
		} else
		{
			// 体力がなかったら、撃破される（復活、出現フラグがfalseになる）
			enemy->respawn.isRespawn = false;
			enemy->isArrival = false;

			// ダメージが消える
			enemy->damage.isDamage = false;

			// ダメージ処理を初期化する
			enemy->damage.timer = 30;
		}
	}
}

/// <summary>
/// ダメージ処理を行う
/// </summary>
/// <param name="boss">ボス</param>
void DamageProcess(Boss* boss)
{
	// null を探す
	if (boss == nullptr)
	{
		return;
	}

	// ダメージを受けている（ダメージフラグがtrueである）ときに、ダメージ処理を行う
	if (boss->damage.isDamage)
	{
		// 体力があるなら、ダメージ処理を行う
		if (boss->damage.hp >= -100)
		{
			boss->damage.timer--;

			if (boss->damage.timer <= 0)
			{
				// ダメージが消える（ダメージフラグがfalseになる）
				boss->damage.isDamage = false;

				// ダメージ処理を初期化する
				boss->damage.timer = 30;
			}
		} 
		else
		{

			// 体力がなかったら、撃破される（復活、出現フラグがfalseになる）
			boss->respawn.isRespawn = false;
			boss->isArrival = false;

			// ダメージが消える
			boss->damage.isDamage = false;

			// ダメージ処理を初期化する
			boss->damage.timer = 30;
		}
	}
}

/// <summary>
/// やられたキャラを撃沈させる
/// </summary>
/// <param name="respawn">復活</param>
/// <param name="pos">位置</param>
/// <param name="vel">移動速度</param>
/// <param name="shape">図形</param>
void Sinking(Respawn respawn, Pos2* pos, Vel2* vel, Shape* shape)
{
	// null を探す
	if (pos == nullptr || vel == nullptr || shape == nullptr)
	{
		return;
	}

	if (respawn.isRespawn == false)
	{
		if (respawn.timer >= 120)
		{
			(*vel) = { 0.0f , 8.0f };
		}

		vel->y -= 0.5f;

		shape->translate.y += vel->y;
		pos->world = VertexAffineMatrix(*shape);
	}
}

/// <summary>
/// ヒットストップを処理する
/// </summary>
/// <param name="hitStop">ヒットストップ</param>
void HitStopProcess(HitStop* hitStop)
{
	// null を探す
	if (hitStop == nullptr)
	{
		return;
	}

	// ヒットストップが発生している（ヒットストップフラグがtrueである）ときに処理を行う
	if (hitStop->isStop)
	{
		hitStop->timer--;

		if (hitStop->timer <= 0)
		{
			// ヒットストップを停止する（ヒットストップフラグをfalseにする）
			hitStop->isStop = false;

			// ヒットストップ処理を初期化する
			hitStop->timer = 15;
		}
	}
}