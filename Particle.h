#pragma once

/// <summary>
/// パーティクルを放出する
/// </summary>
/// <param name="particle">パーティクル</param>
/// <param name="type">種類</param>
void ParticleEmission(Particle* particle, int type, float posX, float posY);

/// <summary>
/// パーティクルを動かす
/// </summary>
/// <param name="particle"></param>
void PartivleMove(Particle* particle);