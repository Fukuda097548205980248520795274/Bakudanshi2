#pragma once

/// <summary>
/// 弾を発射する
/// </summary>
/// <param name="bullet">弾</param>
/// <param name="type">種類</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
void BulletShot(Bullet* bullet, int type, float posX, float posY);

/// <summary>
/// 弾を動かす
/// </summary>
/// <param name="bullet">弾</param>
void BulletMove(Bullet* bullet, Particle* particle);