#include <Novice.h>
#include <math.h>
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
#include "shake.h"
#include "Particle.h"

const char kWindowTitle[] = "1228_爆男子";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kScreenWtidh, kScreenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*------------------------------
		変数を作り、初期値を入れる
	------------------------------*/

	// 初期状態はタイトル画面
	SCENE gameState = SCENE_TITLE;

	/*   メニューなど   */

	// 現在のメニュー
	MENU menuNo = MENU_GAME_START;

	// ゲームフレーム
	int gameFrame = 0;

	// ゲームが止まるかどうか（ストップフラグ）
	int isGameStop = false;


	// プレイヤー、爆弾、弾、敵、ボスの初期化コード
	Player player;
	Bomb bomb[kBombNum];
	Bullet bullet[kBulletNum];
	Enemy enemy[kEnemyNum];
	Boss boss;
	BackGround backGround;

	/*   プレイヤー   */

	// 復活
	player.respawn.isRespawn = true;
	player.respawn.timer = 120;

	// 体力
	player.damage.hp = 0;
	player.damage.isDamage = false;
	player.damage.timer = 30;

	// 爆弾を持っているかどうか（爆弾所持フラグ）
	player.isBomHave = false;

	// 向ている方向
	player.directionNo = -1;

	// 位置
	player.pos.world = { {0.0f , 0.0f} , {0.0f , 0.0f} , {0.0f , 0.0f} , {0.0f , 0.0f} };
	player.pos.screen = CoordinateTransformation(player.pos.world);

	// 移動速度
	player.vel = { 0.0f , 0.0f };

	// ジャンプ
	player.jump.isJump = false;
	player.jump.count = 0;
	player.jump.fallingVel = 0.0f;

	// 図形
	player.shape.scale = { 16.0f , 16.0f };
	player.shape.theta = 0.0f;
	player.shape.translate = { 0.0f , 0.0f };


	/*   爆弾   */

	for (int i = 0; i < kBombNum; i++)
	{
		// 復活
		bomb[i].respawn.isRespawn = true;
		bomb[i].respawn.timer = 120;

		// 起動しているかどうか（起動フラグ）
		bomb[i].isBoot = false;

		// 発射されているかどうか（発射フラグ）
		bomb[i].isShot = false;

		// 制限時間
		bomb[i].timeLimit = 0;

		// 図形
		bomb[i].shape.scale = { 0.0f , 0.0f };
		bomb[i].shape.theta = 0.0f;
		bomb[i].shape.translate = { 0.0f , 0.0f };

		// 位置
		bomb[i].pos.world = VertexAffineMatrix(bomb[i].shape);
		bomb[i].pos.screen = CoordinateTransformation(bomb[i].pos.world);

		// 移動速度
		bomb[i].vel = { 0.0f , 0.0f };

		// 加速度
		bomb[i].acceleration = { 0.0f , 0.0f };
	}


	/*   弾   */

	for (int i = 0; i < kBulletNum; i++)
	{
		// 復活
		bullet[i].respawn.isRespawn = true;
		bullet[i].respawn.timer = 120;

		// 発射しているかどうか（発射フラグ）
		bullet[i].isShot = false;

		// フレーム
		bullet[i].frame = 0;

		// 種類
		bullet[i].type = -1;

		// 図形
		bullet[i].shape.scale = { 0.0f , 0.0f };
		bullet[i].shape.theta = 0.0f;
		bullet[i].shape.translate = { 0.0f , 0.0f };

		// 位置
		bullet[i].pos.world = VertexAffineMatrix(bullet[i].shape);
		bullet[i].pos.screen = CoordinateTransformation(bullet[i].pos.world);

		// 移動速度
		bullet[i].vel = { 0.0f , 0.0f };
	}


	/*   敵   */

	for (int i = 0; i < kEnemyNum; i++)
	{
		// 復活
		enemy[i].respawn.isRespawn = true;
		enemy[i].respawn.timer = 120;

		// ダメージ
		enemy[i].damage.hp = 0;
		enemy[i].damage.isDamage = false;
		enemy[i].damage.timer = 30;

		// 出現しているかどうか（出現フラグ）
		enemy[i].isArrival = false;

		// フレーム
		enemy[i].frame = 0;

		// 種類
		enemy[i].type = -1;

		// 向いている方向
		enemy[i].directionNo = -1;

		// 図形
		enemy[i].shape.scale = { 0.0f , 0.0f };
		enemy[i].shape.theta = 0.0f;
		enemy[i].shape.translate = { 0.0f , 0.0f };

		// 位置
		enemy[i].pos.world = VertexAffineMatrix(enemy[i].shape);
		enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);

		// ベクトル
		enemy[i].vec = { 0.0f , 0.0f };

		// 移動速度
		enemy[i].vel = { 0.0f , 0.0f };
	}


	/*   ボス   */

	// 復活
	boss.respawn.isRespawn = true;
	boss.respawn.timer = 120;

	// ダメージ
	boss.damage.hp = 0;
	boss.damage.isDamage = false;
	boss.damage.timer = 30;

	// 出現しているかどうか（出現フラグ）
	boss.isArrival = false;

	// フレーム
	boss.frame = 0;

	// 種類
	boss.type = -1;

	// 向いている方向
	boss.directionNo = -1;

	// 攻撃しているかどうか（攻撃フラグ）
	boss.isAttack = false;

	// 現在の攻撃
	boss.attackNo = -1;

	// 図形
	boss.shape.scale = { 0.0f , 0.0f };
	boss.shape.theta = 0.0f;
	boss.shape.translate = { 0.0f , 0.0f };

	// 位置
	boss.pos.world = VertexAffineMatrix(boss.shape);
	boss.pos.screen = CoordinateTransformation(boss.pos.world);

	// 移動速度
	boss.vel = { 0.0f , 0.0f };

	// ジャンプ
	boss.jump.isJump = false;
	boss.jump.fallingVel = 0.0f;
	boss.jump.count = 0;


	// ボスのシェイクの強さ
	boss.shakeStrong = { 100,100 };
	boss.shakeWeak = { 50,50 };

	// ボスのシェイクの長さ
	boss.shakeFrame = 50;


	/*   パーティクル   */

	// 構造体
	Particle particle[kParticleNum];

	for (int i = 0; i < kParticleNum; i++)
	{
		// 放出しているかどうか（放出フラグ）
		particle[i].isEmission = false;

		// 放出する時間
		particle[i].emitTimer = 0;

		// 放出する開始時間
		particle[i].emitStartTimer = 0;

		// 種類
		particle[i].type = -1;

		// 図形
		particle[i].shape.scale = { 0.0f , 0.0f };
		particle[i].shape.theta = 0.0f;
		particle[i].shape.translate = { 0.0f , 0.0f };

		// 位置
		particle[i].pos.world = VertexAffineMatrix(particle[i].shape);
		particle[i].pos.screen = CoordinateTransformation(particle[i].pos.world);

		// ベクトル
		particle[i].vec = { 0.0f , 0.0f };

		// 移動速度
		particle[i].vel = { 0.0f , 0.0f };
	}


	/*   コントローラーの左スティック   */

	// 構造体
	LeftStick leftStick;

	// Xが左右、Yが上下
	leftStick.x = 0;
	leftStick.y = 0;

	/*   背景   */

	// シェイクの大きさ
	backGround.shakeRange = { 0 , 0 };

	// シェイクのスピード
	backGround.randSpeed = { 0 , 0 };

	// 乱数での移動距離
	backGround.rand = { 0 , 0 };

	// シェイクしているか
	backGround.isShake = false;


	/*   ヒットストップ   */

	// 構造体
	HitStop hitStop;

	hitStop.isStop = false;
	hitStop.timer = 15;


	/*   画像   */

	// 白い図形
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");

	// ステージ背景
	int ghBgStage = Novice::LoadTexture("./Resources/images/bg/stage.png");

	// 爆弾
	int ghItemBomb = Novice::LoadTexture("./Resources/images/item/bomb.png");

	// プレイヤー
	int ghCharPlayer = Novice::LoadTexture("./Resources/images/char/player.png");

	// ボス
	int ghCharBoss = Novice::LoadTexture("./Resources/images/char/boss.png");

	// 敵（地上）
	int ghCharEnemyGround = Novice::LoadTexture("./Resources/images/char/enemy_ground.png");

	// 敵（空中）
	int ghCharEnemyFlying = Novice::LoadTexture("./Resources/images/char/enemy_flying.png");


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// Rキーでタイトル画面に戻る
		if (keys[DIK_R] && !preKeys[DIK_R]) {
			gameState = SCENE_TITLE;
		}

		// シーンごとの処理
		switch (gameState)
		{
		case SCENE_TITLE:

			// スペースキーでメニューへ
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				gameState = SCENE_MENU;

				gameFrame = 0;
			}

			// タイトル画面の描画
			Novice::ScreenPrintf(100, 100, "title");

			break;

		case SCENE_MENU:

			/// 更新処理

			// 30フレームまで進める
			if (isGameStop == false)
			{
				if (gameFrame < 30)
				{
					gameFrame++;
				}
			}

			switch (menuNo)
			{
			case MENU_GAME_START:

				// スペースキーで、ゲームを始める
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
				{
					if (isGameStop == false)
					{
						isGameStop = true;
					}
				}

				if (isGameStop)
				{
					gameFrame++;

					if (gameFrame >= 90)
					{
						PlayerInitialValue(&player);

						// ボスを出現させる
						BossArrival(&boss, BOSS_TYPE_STAGE_1);

						gameState = SCENE_GAME;

						isGameStop = false;
					}
				}

				if (!preKeys[DIK_S] && keys[DIK_S])
				{
					if (isGameStop == false)
					{
						menuNo = MENU_TUTORIAL;
					}
				}

				break;

			case MENU_TUTORIAL:

				if (!preKeys[DIK_W] && keys[DIK_W])
				{
					if (isGameStop == false)
					{
						menuNo = MENU_GAME_START;
					}
				}

				if (!preKeys[DIK_S] && keys[DIK_S])
				{
					if (isGameStop == false)
					{
						menuNo = MENU_RETURN;
					}
				}

				break;

			case MENU_RETURN:

				if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
				{
					if (isGameStop == false)
					{
						isGameStop = true;

						gameFrame = 30;
					}
				}

				if (isGameStop)
				{
					gameFrame--;

					if (gameFrame <= 0)
					{
						gameState = SCENE_TITLE;

						menuNo = MENU_GAME_START;

						isGameStop = false;

						gameFrame = 0;
					}
				}

				if (!preKeys[DIK_W] && keys[DIK_W])
				{
					if (isGameStop == false)
					{
						menuNo = MENU_TUTORIAL;
					}
				}

				break;
			}


			/// 描画処理

			// ゲームスタート
			if (gameFrame >= 0 && gameFrame <= 30)
			{
				int startFrame = gameFrame;
				int endFrame = 30;

				float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

				float easedT = powf(frameRate, 4);

				Novice::DrawBox
				(
					static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 900.0f) - 150, 200 - 50, 300, 100,
					0.0f, 0xFFFFFFFF, kFillModeSolid
				);
			} else
			{
				Novice::DrawBox
				(
					900 - 150, 200 - 50, 300, 100,
					0.0f, 0xFFFFFFFF, kFillModeSolid
				);
			}

			// チュートリアル
			if (gameFrame >= 0 && gameFrame <= 30)
			{
				int startFrame = gameFrame;
				int endFrame = 30;

				float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

				float easedT = powf(frameRate, 4);

				Novice::DrawBox
				(
					static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 900.0f) - 150, 400 - 50, 300, 100,
					0.0f, 0xFFFFFFFF, kFillModeSolid
				);
			} else
			{
				Novice::DrawBox
				(
					900 - 150, 400 - 50, 300, 100,
					0.0f, 0xFFFFFFFF, kFillModeSolid
				);
			}

			// 選択中
			if (menuNo == MENU_GAME_START)
			{
				// ゲームスタート
				if (gameFrame >= 0 && gameFrame <= 30)
				{
					int startFrame = gameFrame;
					int endFrame = 30;

					float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

					float easedT = powf(frameRate, 4);

					Novice::DrawBox
					(
						static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 900.0f) - 150, 200 - 50, 300, 100,
						0.0f, 0xFF0000FF, kFillModeSolid
					);
				}
			} else if (menuNo == MENU_TUTORIAL)
			{
				// チュートリアル
				if (gameFrame >= 0 && gameFrame <= 30)
				{
					int startFrame = gameFrame;
					int endFrame = 30;

					float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

					float easedT = powf(frameRate, 4);

					Novice::DrawBox
					(
						static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 900.0f) - 150, 400 - 50, 300, 100,
						0.0f, 0xFF0000FF, kFillModeSolid
					);
				}
			}


			break;

		case SCENE_GAME:

			// ゲームのメイン処理

			if (gameFrame < 150)
			{
				gameFrame++;
			}

			// 150フレームでゲームが動き出す
			if (gameFrame >= 150)
			{
				/*----------------------
					復活、ダメージ処理
				----------------------*/

				// ヒットストップ処理
				HitStopProcess(&hitStop);

				if (hitStop.isStop == false)
				{
					// プレイヤー
					if (player.respawn.timer >= 10)
					{
						Sinking(player.respawn, &player.pos, &player.vel, &player.shape);
						RespawnProcess(&player.respawn);
						DamageProcess(&player);
					} else
					{
						gameFrame++;
					}

					// 爆弾
					for (int i = 0; i < kBombNum; i++)
					{
						RespawnProcess(&bomb[i].respawn);
					}

					// 弾
					for (int i = 0; i < kBulletNum; i++)
					{
						RespawnProcess(&bullet[i].respawn);
					}

					// 敵
					for (int i = 0; i < kEnemyNum; i++)
					{
						Sinking(enemy[i].respawn, &enemy[i].pos, &enemy[i].vel, &enemy[i].shape);
						RespawnProcess(&enemy[i].respawn);
						DamageProcess(&enemy[i]);
					}

					if (boss.respawn.timer >= 10)
					{
						// ボス
						Sinking(boss.respawn, &boss.pos, &boss.vel, &boss.shape);
						RespawnProcess(&boss.respawn);
						DamageProcess(&boss);
					} else
					{
						gameFrame++;
					}
				}


				/*-------------------
					動きを設定する
				-------------------*/

				if (hitStop.isStop == false)
				{
					// プレイヤーを操作する
					PlayerMove(&player, keys, preKeys, &leftStick);

					// プレイヤーが爆弾を使う
					PlayerBombUse(&player, bomb, bullet, keys, preKeys);

					// 爆弾を動かす
					BombMove(bomb, bullet , particle);

					// 弾を動かす
					BulletMove(bullet , particle);

					// 敵を動かす
					EnemyMove(enemy, &player , particle);

					// ボスを動かす
					BossMove(&boss, &player, bullet, enemy, particle, &backGround);

					// パーティクルを動かす
					PartivleMove(particle);

					// シェイク
					ShakeBG(&backGround);
				}

				/*-----------------
					当たり判定
				-----------------*/

				// プレイヤー と 爆発の判定など
				// 以下に全ての当たり判定処理を実装

				// プレイヤー と 爆発
				if (player.respawn.isRespawn)
				{
					for (int i = 0; i < kBulletNum; i++)
					{
						if (bullet[i].isShot)
						{
							if (HitBox(player.shape, bullet[i].shape))
							{
								switch (bullet[i].type)
								{
								case BULLET_TYPE_EXPLOSION:

									if (player.damage.isDamage == false)

									{
										player.damage.hp -= 50;
										player.damage.isDamage = true;

										hitStop.isStop = true;
									}

									break;

								case BULLET_TYPE_VIBRATION_LEFT:

									if (player.damage.isDamage == false)
									{
										player.damage.hp -= 50;
										player.damage.isDamage = true;

										hitStop.isStop = true;
									}

									break;

								case BULLET_TYPE_VIBRATION_RIGHT:

									if (player.damage.isDamage == false)
									{
										player.damage.hp -= 50;
										player.damage.isDamage = true;

										hitStop.isStop = true;
									}

									break;

								case BULLET_TYPE_RUBBLE:

									if (player.damage.isDamage == false)
									{
										player.damage.hp -= 100;
										player.damage.isDamage = true;

										hitStop.isStop = true;
									}

									break;
								}
							}
						}
					}
				}

				// プレイヤー と 敵
				if (player.respawn.isRespawn)
				{
					for (int i = 0; i < kEnemyNum; i++)
					{
						if (enemy[i].isArrival)
						{
							if (HitBox(player.shape, enemy[i].shape))
							{
								if (player.damage.isDamage == false)
								{
									player.damage.hp -= 25;
									player.damage.isDamage = true;

									hitStop.isStop = true;
								}
							}
						}
					}
				}

				// プレイヤー と ボス
				if (player.respawn.isRespawn)
				{
					if (boss.isArrival)
					{
						if (HitBox(player.shape, boss.shape))
						{
							switch (boss.attackNo)
							{
							case 0:

								if (boss.frame == 20)
								{
									if (player.damage.isDamage == false)
									{
										player.damage.hp -= 150;
										player.damage.isDamage = true;

										hitStop.isStop = true;
									}
								}

								break;

							default:

								if (player.damage.isDamage == false)
								{
									player.damage.hp -= 50;
									player.damage.isDamage = true;

									hitStop.isStop = true;
								}
							}
						}
					}
				}

				// 敵 と 爆発
				for (int i = 0; i < kEnemyNum; i++)
				{
					if (enemy[i].isArrival)
					{
						for (int j = 0; j < kBulletNum; j++)
						{
							if (bullet[j].isShot)
							{
								if (bullet[j].type == BULLET_TYPE_EXPLOSION)
								{
									if (HitBox(enemy[i].shape, bullet[j].shape))
									{
										if (enemy[i].damage.isDamage == false)
										{
											enemy[i].damage.hp -= 50;
											enemy[i].damage.isDamage = true;
										}
									}
								}
							}
						}
					}
				}

				// ボス と 爆発
				if (boss.isArrival)
				{
					for (int i = 0; i < kBulletNum; i++)
					{
						if (bullet[i].isShot)
						{
							if (bullet[i].type == BULLET_TYPE_EXPLOSION)
							{
								if (HitBox(boss.shape, bullet[i].shape))
								{
									if (boss.damage.isDamage == false)
									{
										boss.damage.hp -= 50;
										boss.damage.isDamage = true;
									}
								}
							}
						}
					}
				}
			}

			if (gameFrame >= 220)
			{
				if (player.respawn.isRespawn == false)
				{
					gameState = SCENE_GAME_OVER;
				}

				if (boss.respawn.isRespawn == false)
				{
					gameState = SCENE_GAME_CLEAR;
				}
			}

			/*-------------
				座標変換
			-------------*/

			// プレイヤー
			player.pos.screen = CoordinateTransformation(player.pos.world);

			// 爆弾
			for (int i = 0; i < kBombNum; i++) {
				bomb[i].pos.screen = CoordinateTransformation(bomb[i].pos.world);
			}

			// 弾
			for (int i = 0; i < kBulletNum; i++) {
				bullet[i].pos.screen = CoordinateTransformation(bullet[i].pos.world);
			}

			// 敵
			for (int i = 0; i < kEnemyNum; i++) {
				enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);
			}

			// ボス
			boss.pos.screen = CoordinateTransformation(boss.pos.world);

			// パーティクル
			for (int i = 0; i < kParticleNum; i++)
			{
				particle[i].pos.screen = CoordinateTransformation(particle[i].pos.world);
			}

			///* ゲームオーバー、クリア判定 */
			//if (player.damage.hp <= 0) {
			//    gameState = GAME_OVER;
			//} else if (boss.isDefeated) {
			//    gameState = GAME_CLEAR;
			//}        

			/* 描画処理 */
			// ここにゲーム中の描画処理を配置7

			Novice::DrawSprite(-100 + backGround.rand.x, -100 + backGround.rand.y, ghBgStage, 1, 1, 0.0f, 0xFFFFFFFF);

			// 放出された（放出フラグがtrueである）パーティクル
			for (int i = 0; i < kParticleNum; i++)
			{
				if (particle[i].isEmission)
				{
					Novice::DrawQuad
					(
						static_cast<int>(particle[i].pos.screen.leftTop.x) , static_cast<int>(particle[i].pos.screen.leftTop.y) ,
						static_cast<int>(particle[i].pos.screen.rightTop.x), static_cast<int>(particle[i].pos.screen.rightTop.y),
						static_cast<int>(particle[i].pos.screen.leftBottom.x), static_cast<int>(particle[i].pos.screen.leftBottom.y),
						static_cast<int>(particle[i].pos.screen.rightBottom.x), static_cast<int>(particle[i].pos.screen.rightBottom.y),
						0,0,1,1,ghWhite,
						0xFFFFFF00 + static_cast<int>(255.0f * (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer)))
					);
				}
			}
			

			// ボス
			if (boss.respawn.isRespawn)
			{
				if (boss.isArrival)
				{
					if (boss.directionNo == DIRECTION_LEFT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
							static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
							static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
							static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
							0, 0, 742, 1069, ghCharBoss, 0xFFFFFFFF
						);
					} else if (boss.directionNo == DIRECTION_RIGHT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
							static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
							static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
							static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
							0, 0, 742, 1069, ghCharBoss, 0xFFFFFFFF
						);
					}
				}
			} else
			{
				if (boss.directionNo == DIRECTION_LEFT)
				{
					Novice::DrawQuad
					(
						static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
						static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
						static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
						static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
						0, 0, 742, 1069, ghCharBoss, 0xFFFFFFFF
					);
				} else if (boss.directionNo == DIRECTION_RIGHT)
				{
					Novice::DrawQuad
					(
						static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
						static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
						static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
						static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
						0, 0, 742, 1069, ghCharBoss, 0xFFFFFFFF
					);
				}
			}

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				if (enemy[i].respawn.isRespawn)
				{
					if (enemy[i].isArrival)
					{
						switch (enemy[i].type)
						{
						case ENEMY_TYPE_GROUND:

							if (enemy[i].directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
									static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
									static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
									static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
									0, 0, 1024, 1024, ghCharEnemyGround, 0xFFFFFFFF
								);
							} else if (enemy[i].directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
									static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
									static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
									static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
									0, 0, 1024, 1024, ghCharEnemyGround, 0xFFFFFFFF
								);
							}

							break;

						case ENEMY_TYPE_FLYING:

							Novice::DrawQuad
							(
								static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
								static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
								static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
								static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
								0, 0, 1024, 1024, ghCharEnemyFlying, 0xFFFFFFFF
							);

							break;
						}
					}
				} else
				{
					switch (enemy[i].type)
					{
					case ENEMY_TYPE_GROUND:

						if (enemy[i].directionNo == DIRECTION_LEFT)
						{
							Novice::DrawQuad
							(
								static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
								static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
								static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
								static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
								0, 0, 1024, 1024, ghCharEnemyGround, 0xFFFFFFFF
							);
						} else if (enemy[i].directionNo == DIRECTION_RIGHT)
						{
							Novice::DrawQuad
							(
								static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
								static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
								static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
								static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
								0, 0, 1024, 1024, ghCharEnemyGround, 0xFFFFFFFF
							);
						}

						break;

					case ENEMY_TYPE_FLYING:

						Novice::DrawQuad
						(
							static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
							static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
							static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
							static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
							0, 0, 1024, 1024, ghCharEnemyFlying, 0xFFFFFFFF
						);

						break;
					}
				}
			}

			// 弾
			for (int i = 0; i < kBulletNum; i++)
			{
				if (bullet[i].isShot)
				{
					if (bullet[i].type != BULLET_TYPE_EXPLOSION)
					{
						Novice::DrawQuad
						(
							static_cast<int>(bullet[i].pos.screen.leftTop.x), static_cast<int>(bullet[i].pos.screen.leftTop.y),
							static_cast<int>(bullet[i].pos.screen.rightTop.x), static_cast<int>(bullet[i].pos.screen.rightTop.y),
							static_cast<int>(bullet[i].pos.screen.leftBottom.x), static_cast<int>(bullet[i].pos.screen.leftBottom.y),
							static_cast<int>(bullet[i].pos.screen.rightBottom.x), static_cast<int>(bullet[i].pos.screen.rightBottom.y),
							0, 0, 1, 1, ghWhite, 0xFFFFFFFF
						);
					}
				}
			}

			// 爆弾
			for (int i = 0; i < kBombNum; i++)
			{
				if (bomb[i].isBoot || bomb[i].isShot)
				{
					Novice::DrawQuad
					(
						static_cast<int>(bomb[i].pos.screen.leftTop.x), static_cast<int>(bomb[i].pos.screen.leftTop.y),
						static_cast<int>(bomb[i].pos.screen.rightTop.x), static_cast<int>(bomb[i].pos.screen.rightTop.y),
						static_cast<int>(bomb[i].pos.screen.leftBottom.x), static_cast<int>(bomb[i].pos.screen.leftBottom.y),
						static_cast<int>(bomb[i].pos.screen.rightBottom.x), static_cast<int>(bomb[i].pos.screen.rightBottom.y),
						0, 0, 1024, 1024, ghItemBomb, 0xFFFFFFFF
					);
				}
			}


			// プレイヤー
			if (player.respawn.isRespawn)
			{
				if (player.damage.isDamage == false)
				{
					if (player.directionNo == DIRECTION_LEFT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
							static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
							static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
							static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
							0, 0, 1024, 1024, ghCharPlayer, 0xFFFFFFFF
						);
					} else if (player.directionNo == DIRECTION_RIGHT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
							static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
							static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
							static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
							0, 0, 1024, 1024, ghCharPlayer, 0xFFFFFFFF
						);
					}
				} else
				{
					if (player.directionNo == DIRECTION_LEFT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
							static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
							static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
							static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
							0, 0, 1024, 1024, ghCharPlayer, 0xFF0000FF
						);
					} else if (player.directionNo == DIRECTION_RIGHT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
							static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
							static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
							static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
							0, 0, 1024, 1024, ghCharPlayer, 0xFF0000FF
						);
					}
				}
			} else
			{
				if (player.directionNo == DIRECTION_LEFT)
				{
					Novice::DrawQuad
					(
						static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
						static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
						static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
						static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
						0, 0, 1024, 1024, ghCharPlayer, 0xFFFFFFFF
					);
				} else if (player.directionNo == DIRECTION_RIGHT)
				{
					Novice::DrawQuad
					(
						static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
						static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
						static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
						static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
						0, 0, 1024, 1024, ghCharPlayer, 0xFFFFFFFF
					);
				}
			}

			Novice::ScreenPrintf(100, 100, "%d", player.damage.hp);

			break;

		case SCENE_GAME_OVER:

			Novice::ScreenPrintf(100, 100, "game over");

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				gameState = SCENE_TITLE;
			}

			break;

		case SCENE_GAME_CLEAR:

			Novice::ScreenPrintf(100, 100, "game clear");

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				gameState = SCENE_TITLE;
			}

			break;
		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
