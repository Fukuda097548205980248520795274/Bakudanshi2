#pragma once

/// <summary>
/// プレイヤーが爆弾を起動する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="bomb">爆弾</param>
void PlayerBombBoot(Player* player, Bomb* bomb);

/// <summary>
/// 爆弾を起動する
/// </summary>
/// <param name="bomb"></param>
void BombBootAndShot(Bomb* bomb, float posX, float posY, float radius);

/// <summary>
/// 爆弾を発射する
/// </summary>
/// <param name="bomb">爆弾</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="accelerationX">X軸の加速度</param>
/// <param name="accelerationY">Y軸の加速度</param>
void PlayerBombShot(Bomb* bomb, float velX, float velY, float accelerationX, float accelerationY);

/// <summary>
/// 爆弾を動かす
/// </summary>
/// <param name="bomb">爆弾</param>
/// <param name="bullet">弾</param>
void BombMove(Bomb* bomb, Bullet* bullet, Particle* particle, SH* sh);