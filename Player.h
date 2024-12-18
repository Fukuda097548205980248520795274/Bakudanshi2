#pragma once

/// <summary>
/// ゲーム中のプレイヤーの初期値を入れる
/// </summary>
/// <param name="player"></param>
void PlayerInitialValue(Player* player);

/// <summary>
/// プレイヤーの移動操作を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="keys">キー</param>
/// <param name="preKeys">前に押したキー</param>
void PlayerMove(Player* player, Particle* particle, const char* keys, const char* preKeys, LeftStick* leftStick, SH* sh);

/// <summary>
/// プレイヤーが爆弾を使う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="bomb">爆弾</param>
/// <param name="bullet">弾</param>
/// <param name="keys">キー</param>
/// <param name="preKeys">前に押したキー</param>
void PlayerBombUse(Player* player, Bomb* bomb, Bullet* bullet, const char* keys, const char* preKeys, SH* sh, Particle* particle);