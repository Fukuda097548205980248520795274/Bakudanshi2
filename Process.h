#pragma once

#include "Structure.h"

/// <summary>
/// 復活処理を行う
/// </summary>
/// <param name="respawn">復活</param>
void RespawnProcess(Respawn* respawn);

/// <summary>
/// ダメージ処理を行う
/// </summary>
/// <param name="player"></param>
void DamageProcess(Player* player, SH* sh);

/// <summary>
/// ダメージ処理を行う
/// </summary>
/// <param name="enemy">敵</param>
void DamageProcess(Enemy* enemy);

/// <summary>
/// ダメージ処理を行う
/// </summary>
/// <param name="boss">ボス</param>
void DamageProcess(Boss* boss);

/// <summary>
/// やられたキャラを撃沈させる
/// </summary>
/// <param name="respawn">復活</param>
/// <param name="pos">位置</param>
/// <param name="vel">移動速度</param>
/// <param name="shape">図形</param>
void Sinking(Respawn respawn, Pos2* pos, Vel2* vel, Shape* shape);

/// <summary>
/// ヒットストップを処理する
/// </summary>
/// <param name="hitStop">ヒットストップ</param>
void HitStopProcess(HitStop* hitStop);