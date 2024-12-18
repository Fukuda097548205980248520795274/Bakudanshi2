#pragma once

/// <summary>
/// 敵を出現させる
/// </summary>
/// <param name="enemy">敵</param>
/// <param name="type">種類</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
void EnemyArrival(Enemy* enemy, int type, float posX, float posY, SH* sh);

/// <summary>
/// 敵を動かす
/// </summary>
/// <param name="enemy">敵</param>
void EnemyMove(Enemy* enemy, Player* player, Particle* particle);