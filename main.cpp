#include <Novice.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
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
#include "FullScreen.h"

const char kWindowTitle[] = "1228_爆男子";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kScreenWtidh, kScreenHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 乱数
	unsigned int currentTimer = static_cast<unsigned int>(time(nullptr));
	srand(currentTimer);

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
	SH sH;

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

	// フレーム
	player.frame = 0;

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


	// 突進のフレーム
	int rushFrame = 0;


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


	/*   アイテム   */

	// 構造体
	Item item[kItemNum];

	for (int i = 0; i < kItemNum; i++)
	{
		item[i].isShot = false;

		item[i].type = -1;

		item[i].shape.scale = { 0.0f , 0.0f };
		item[i].shape.theta = 0.0f;
		item[i].shape.translate = { 0.0f , 0.0f };

		item[i].pos.world = VertexAffineMatrix(item[i].shape);
		item[i].pos.screen = CoordinateTransformation(item[i].pos.world);

		item[i].vel = { 0.0f , 0.0f };

		item[i].acceleration = { 0.0f , 0.0f };
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

	// タイトル
	int ghMenu = Novice::LoadTexture("./Resources/images/menu/menu.png");
	int ghGameOver = Novice::LoadTexture("./Resources/images/menu/gameOver.png");
	int ghGameClear = Novice::LoadTexture("./Resources/images/menu/gameClear.png");

	// メニュー
	int ghMenuExit = Novice::LoadTexture("./Resources/images/menu/menuExit.png");
	int ghMenuStart = Novice::LoadTexture("./Resources/images/menu/menuStart.png");

	// ステージ背景
	int ghBgStage = Novice::LoadTexture("./Resources/images/bg/stage.png");

	// 爆弾
	int ghItemBomb = Novice::LoadTexture("./Resources/images/item/bomb.png");

	// 岩
	int ghStone = Novice::LoadTexture("./Resources/images/item/rock.png");
	int ghWave = Novice::LoadTexture("./Resources/images/item/wave.png");

	// プレイヤー
	int ghCharPlayer = Novice::LoadTexture("./Resources/images/char/player.png");
	int ghCharPlayerRun = Novice::LoadTexture("./Resources/images/char/playerRun.png");
	int ghCharPlayerBomb = Novice::LoadTexture("./Resources/images/char/playerBom.png");
	int ghCharPlayerBombRun = Novice::LoadTexture("./Resources/images/char/playerBomRun.png");

	// ボス
	int ghCharBoss = Novice::LoadTexture("./Resources/images/char/boss.png");
	int ghCharBossRun = Novice::LoadTexture("./Resources/images/char/bossRun.png");
	int ghCharBossJump = Novice::LoadTexture("./Resources/images/char/bossJump.png");
	int ghCharBossRoof = Novice::LoadTexture("./Resources/images/char/bossRoof.png");
	int ghCharBossEnemyGround = Novice::LoadTexture("./Resources/images/char/bossEnemyGround.png");
	int ghCharBossEnemySky = Novice::LoadTexture("./Resources/images/char/bossEnemySky.png");

	// 敵（地上）
	int ghCharEnemyGround = Novice::LoadTexture("./Resources/images/char/enemy_ground.png");

	// 敵（空中）
	int ghCharEnemyFlying = Novice::LoadTexture("./Resources/images/char/enemy_flying.png");

	/*   効果音   */

	// 歩く
	sH.playerWalk = Novice::LoadAudio("./Resources/Sounds/playerWalk.mp3");

	// ダメージ
	sH.playerDamage = Novice::LoadAudio("./Resources/Sounds/playerDamage.mp3");
	sH.pHPlayerDamage = -1;

	// ジャンプ
	sH.jump = Novice::LoadAudio("./Resources/Sounds/jump.mp3");

	// プレイヤーの着地
	sH.land = Novice::LoadAudio("./Resources/Sounds/land.mp3");

	// プレイヤ死亡
	sH.playerKO = Novice::LoadAudio("./Resources/Sounds/ko.mp3");

	// 爆弾の導火線
	sH.bomCount = Novice::LoadAudio("./Resources/Sounds/bomCount.mp3");
	sH.pHBomCount = -1;
	sH.volumeBomCount = 0.4f;

	// 爆弾の爆発
	sH.explosive = Novice::LoadAudio("./Resources/Sounds/bom.mp3");

	// ボスの突進
	sH.bossDash = Novice::LoadAudio("./Resources/Sounds/bossDash.mp3");
	sH.pHBossDash = -1;

	// 壁にぶつかる
	sH.hitWall = Novice::LoadAudio("./Resources/Sounds/hitWall.mp3");
	sH.pHHitWall = -1;

	// 飛び上がる
	sH.fly = Novice::LoadAudio("./Resources/Sounds/bossFly.mp3");

	// 天井に当たる
	sH.hitSelling = Novice::LoadAudio("./Resources/Sounds/hitCelling.mp3");

	// 落石
	sH.fallingRock = Novice::LoadAudio("./Resources/Sounds/fallingRock.mp3");
	sH.pHfallingRock = -1;

	// 振動
	sH.earthqueke = Novice::LoadAudio("./Resources/Sounds/earthqueke.mp3");
	sH.pHEarthqueke = -1;

	// 地面を叩きつける
	sH.groundSlum = Novice::LoadAudio("./Resources/Sounds/groundslum.mp3");

	// ジャンプ
	sH.bossJump = Novice::LoadAudio("./Resources/Sounds/bossJump.mp3");

	// 雑魚敵召喚
	sH.enemyStart = Novice::LoadAudio("./Resources/Sounds/enemyStart.mp3");
	sH.flyingEnemyStart = Novice::LoadAudio("./Resources/Sounds/flyingEnemyStart.mp3");
	sH.pHEnemyStart = -1;

	// カーソル音
	sH.cursor = Novice::LoadAudio("./Resources/Sounds/cursor.mp3");

	// 決定
	sH.decision = Novice::LoadAudio("./Resources/Sounds/decision.mp3");

	// bgm
	sH.menu = Novice::LoadAudio("./Resources/Sounds/menu.mp3");
	sH.pHmenu = -1;

	sH.title = Novice::LoadAudio("./Resources/Sounds/title.mp3");
	sH.pHtitle = -1;

	sH.battle = Novice::LoadAudio("./Resources/Sounds/battle.mp3");
	sH.pHbattle = -1;

	Titlebar titlebar;
	titlebar.pos = { 640.0f, -400.0f };
	titlebar.velocity = { 0.0f, 10.0f };
	titlebar.acceleration = { 0.0f, 0.1f };
	titlebar.radius = { 300.0f, 200.0f };
	titlebar.jumpCount = 3;
	titlebar.isJump = true;

	// フルスクリーンにする
	SetFullScreen(GetActiveWindow());

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// 左スティックを感知する
		Novice::GetAnalogInputLeft(0, &leftStick.x, &leftStick.y);

		// Rキーでタイトル画面に戻る
		if (keys[DIK_R] && !preKeys[DIK_R]) {
			gameState = SCENE_TITLE;
		}

		// シーンごとの処理
		switch (gameState)
		{
		case SCENE_TITLE:

			if (gameFrame < 600)
			{
				gameFrame++;
			}

			if (gameFrame >= 60 && gameFrame <= 180)
			{
				if (gameFrame % 20 == 0)
				{
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < kItemNum; j++)
						{
							if (item[j].isShot == false)
							{
								item[j].isShot = true;

								item[j].type = ITEM_TYPE_BOM;

								item[j].shape.scale = { 32.0f,32.0f };
								item[j].shape.theta = static_cast<float>(rand() % 360);
								item[j].shape.translate = { static_cast<float>(rand() % kScreenWtidh) , 700.0f };

								item[j].pos.world = VertexAffineMatrix(item[j].shape);

								item[j].vel = { 0.0f , static_cast<float>(rand() % 8 + 4) };

								item[j].acceleration = { 0.0f , 0.0f };

								break;
							}
						}
					}
				}
			}

			for (int i = 0; i < kItemNum; i++)
			{
				if (item[i].isShot)
				{
					if (item[i].acceleration.y > -1.0f)
					{
						item[i].acceleration.y -= 0.1f;
					}

					item[i].shape.translate.y += item[i].vel.y * item[i].acceleration.y;

					item[i].pos.world = VertexAffineMatrix(item[i].shape);
					item[i].pos.screen = CoordinateTransformation(item[i].pos.world);
				}
			}

			if (gameFrame >= 600)
			{
				for (int i = 0; i < kItemNum; i++)
				{
					item[i].isShot = false;
				}
			}

			Novice::ScreenPrintf(100, 300, "%d", titlebar.jumpCount);

			if (titlebar.isJump) {
				// 加速度
				titlebar.velocity.y += titlebar.acceleration.y;

				// 速度
				titlebar.pos.y += titlebar.velocity.y;

				// 跳ね返り
				if (titlebar.pos.y >= 600.0f - titlebar.radius.y) {
					titlebar.pos.y = 600.0f - titlebar.radius.y;
					titlebar.velocity.y *= -0.3f;
					titlebar.jumpCount--;
					//titlebar.isJump = false;
				}
			}

			if (titlebar.jumpCount <= 0) {

				titlebar.isJump = false;

			}

			// スペースキーでメニューへ
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				if (gameFrame >= 600)
				{
					gameState = SCENE_MENU;

					gameFrame = 600;
				} else
				{
					gameFrame = 600;
				}
				Novice::PlayAudio(sH.decision, 0, 0.8f);
			}


			/// 描画処理

			for (int i = 0; i < kItemNum; i++)
			{
				if (item[i].isShot)
				{
					Novice::DrawQuad
					(
						static_cast<int>(item[i].pos.screen.leftTop.x), static_cast<int>(item[i].pos.screen.leftTop.y),
						static_cast<int>(item[i].pos.screen.rightTop.x), static_cast<int>(item[i].pos.screen.rightTop.y),
						static_cast<int>(item[i].pos.screen.leftBottom.x), static_cast<int>(item[i].pos.screen.leftBottom.y),
						static_cast<int>(item[i].pos.screen.rightBottom.x), static_cast<int>(item[i].pos.screen.rightBottom.y),
						0, 0, 1, 1, ghWhite, 0xFFFFFFFF
					);
				}
			}

			Novice::DrawBox
			(
				static_cast<int>(titlebar.pos.x - titlebar.radius.x),
				static_cast<int>(titlebar.pos.y - titlebar.radius.y),
				static_cast<int>(titlebar.radius.x * 2.0f),
				static_cast<int>(titlebar.radius.y * 2.0f),
				0.0f, 0xFFFFFFFF, kFillModeSolid
			);

			if (!Novice::IsPlayingAudio(sH.pHtitle) || sH.pHtitle == -1) {
				sH.pHtitle = Novice::PlayAudio(sH.title, 1, 0.3f);
			}

			Novice::StopAudio(sH.pHmenu);

			break;

		case SCENE_MENU:

			/// 更新処理

			// 630フレームまで進める
			if (isGameStop == false)
			{
				if (gameFrame < 630)
				{
					gameFrame++;
				}
			}

			if (!Novice::IsPlayingAudio(sH.pHmenu) || sH.pHmenu == -1) {
				sH.pHmenu = Novice::PlayAudio(sH.menu, 1, 0.3f);
			}

			Novice::StopAudio(sH.pHtitle);

			switch (menuNo)
			{
			case MENU_GAME_START:

				// スペースキーで、ゲームを始める
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
				{
					if (isGameStop == false)
					{
						isGameStop = true;

						Novice::PlayAudio(sH.decision, 0, 0.8f);
					}
				}

				if (isGameStop)
				{
					gameFrame++;

					if (gameFrame >= 690)
					{
						PlayerInitialValue(&player);

						// ボスを出現させる
						BossArrival(&boss, BOSS_TYPE_STAGE_1);

						gameState = SCENE_GAME;

						isGameStop = false;

					}
				}

				if (!preKeys[DIK_S] && keys[DIK_S] || leftStick.y > 0)
				{
					if (isGameStop == false)
					{
						menuNo = MENU_RETURN;

						Novice::PlayAudio(sH.cursor, 0, 1.0f);
					}
				}

				break;

			case MENU_RETURN:

				if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
				{
					if (isGameStop == false)
					{
						isGameStop = true;

						gameFrame = 630;
					}
				}

				if (isGameStop)
				{
					gameFrame--;

					if (gameFrame <= 600)
					{
						gameState = SCENE_TITLE;

						menuNo = MENU_GAME_START;

						isGameStop = false;

						gameFrame = 600;
					}
				}

				if (!preKeys[DIK_W] && keys[DIK_W] || leftStick.y < 0)
				{
					if (isGameStop == false)
					{
						menuNo = MENU_GAME_START;

						Novice::PlayAudio(sH.cursor, 0, 1.0f);
					}
				}

				break;
			}


			/// 描画処理

			Novice::DrawSprite(0, 0, ghMenu, 1, 1, 0.0f, 0xFFFFFFFF);

			// ゲームスタート
			if (gameFrame >= 600 && gameFrame <= 630)
			{
				int startFrame = gameFrame - 600;
				int endFrame = 30;

				float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

				float easedT = powf(frameRate, 4);

				Novice::DrawSprite
				(
					static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 600.0f) - 150, 320 - 50,
					ghMenuStart,
					1.0f, 1.0f, 0.0f, 0xFFFFFF88
				);

			} else
			{

				Novice::DrawSprite
				(
					600 - 150, 320 - 50,
					ghMenuStart,
					1.0f, 1.0f, 0.0f, 0xFFFFFF88
				);
			}

			// チュートリアル
			if (gameFrame >= 600 && gameFrame <= 630)
			{
				int startFrame = gameFrame - 600;
				int endFrame = 30;

				float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

				float easedT = powf(frameRate, 4);

				Novice::DrawSprite
				(
					static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 600.0f) - 150, 480 - 50,
					ghMenuExit,
					1.0f, 1.0f, 0.0f, 0xFFFFFF88
				);

			} else
			{
				Novice::DrawSprite
				(
					600 - 150, 480 - 50,
					ghMenuExit,
					1.0f, 1.0f, 0.0f, 0xFFFFFF88
				);
			}

			// 選択中
			if (menuNo == MENU_GAME_START)
			{
				// ゲームスタート
				if (gameFrame >= 600 && gameFrame <= 630)
				{
					int startFrame = gameFrame - 600;
					int endFrame = 30;

					float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

					float easedT = powf(frameRate, 4);

					Novice::DrawSprite
					(
						static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 600.0f) - 150, 320 - 50,
						ghMenuStart,
						1.0f, 1.0f, 0.0f, 0xFFFFFFFF
					);
				}
			} else if (menuNo == MENU_RETURN)
			{
				// チュートリアル
				if (gameFrame >= 600 && gameFrame <= 630)
				{
					int startFrame = gameFrame - 600;
					int endFrame = 30;

					float frameRate = static_cast<float>(startFrame) / static_cast<float>(endFrame);

					float easedT = powf(frameRate, 4);

					Novice::DrawSprite
					(
						static_cast<int>((1.0f - easedT) * 1500.0f + easedT * 600.0f) - 150, 480 - 50,
						ghMenuExit,
						1.0f, 1.0f, 0.0f, 0xFFFFFFFF
					);
				}
			}


			break;

		case SCENE_GAME:

			// ゲームのメイン処理

			if (gameFrame < 750)
			{
				gameFrame++;
			}

			// 150フレームでゲームが動き出す
			if (gameFrame >= 750)
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
					PlayerMove(&player, particle, keys, preKeys, &leftStick, &sH);

					// プレイヤーが爆弾を使う
					PlayerBombUse(&player, bomb, bullet, keys, preKeys, &sH, particle);

					// 爆弾を動かす
					BombMove(bomb, bullet, particle, &sH);

					// 弾を動かす
					BulletMove(bullet, particle);

					// 敵を動かす
					EnemyMove(enemy, &player, particle);

					// ボスを動かす
					BossMove(&boss, &player, bullet, enemy, particle, &backGround, &sH);

					rushFrame++;

					if (boss.attackNo == 0)
					{
						if (boss.frame == 54)
						{
							if (rushFrame % 5 == 0)
							{
								if (boss.directionNo == DIRECTION_LEFT)
								{
									ParticleEmission(particle, PARTICLE_TYPE_SPARK_RIGHT, boss.shape.translate.x + boss.shape.scale.x, boss.shape.translate.y - boss.shape.scale.y);
								} else if (boss.directionNo == DIRECTION_RIGHT)
								{
									ParticleEmission(particle, PARTICLE_TYPE_SPARK_LEFT, boss.shape.translate.x - boss.shape.scale.x, boss.shape.translate.y - boss.shape.scale.y);
								}
							}
						}
					}

					if (rushFrame >= 20)
					{
						rushFrame = 0;
					}

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

								if (boss.frame == 54)
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

			if (player.damage.isDamage) {
				if (!Novice::IsPlayingAudio(sH.pHPlayerDamage) || sH.pHPlayerDamage == -1) {
					sH.pHPlayerDamage = Novice::PlayAudio(sH.playerDamage, 0, 0.6f);
				}
			}

			if (gameFrame >= 820)
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

			Novice::DrawSprite(-120 + backGround.rand.x, -120 + backGround.rand.y, ghBgStage, 1, 1, 0.0f, 0xFFFFFFFF);

			// 放出された（放出フラグがtrueである）パーティクル
			for (int i = 0; i < kParticleNum; i++)
			{
				if (particle[i].isEmission)
				{
					Novice::DrawQuad
					(
						static_cast<int>(particle[i].pos.screen.leftTop.x), static_cast<int>(particle[i].pos.screen.leftTop.y),
						static_cast<int>(particle[i].pos.screen.rightTop.x), static_cast<int>(particle[i].pos.screen.rightTop.y),
						static_cast<int>(particle[i].pos.screen.leftBottom.x), static_cast<int>(particle[i].pos.screen.leftBottom.y),
						static_cast<int>(particle[i].pos.screen.rightBottom.x), static_cast<int>(particle[i].pos.screen.rightBottom.y),
						0, 0, 1, 1, ghWhite,
						0xFFFFFF00 + static_cast<int>(255.0f * (static_cast<float>(particle[i].emitTimer) / static_cast<float>(particle[i].emitStartTimer)))
					);
				}
			}


			// ボス
			if (boss.isArrival)
			{
				if (boss.isAttack)
				{
					switch (boss.attackNo)
					{
					case 0:

						if (boss.frame < 24)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						if (boss.frame >= 24 && boss.frame <= 55)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									64 * ((boss.frame - 24) / 8), 0, 64, 128, ghCharBossRun, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									64 * ((boss.frame - 24) / 8), 0, 64, 128, ghCharBossRun, 0xFFFFFFFF
								);
							}
						}

						if (boss.frame > 55)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						break;

					case 1:

						if (boss.frame < 25)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						if (boss.frame >= 25 && boss.frame <= 48)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								64 * ((boss.frame - 25) / 6), 0, 64, 128, ghCharBossJump, 0xFFFFFFFF
							);
						}

						if (boss.frame > 48 && boss.frame < 60)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								64 * 3, 0, 64, 128, ghCharBossJump, 0xFFFFFFFF
							);
						}

						if (boss.frame >= 60 && boss.frame < 160)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						if (boss.frame >= 160 && boss.frame < 310)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								64 * 3, 0, 64, 128, ghCharBossJump, 0xFFFFFFFF
							);
						}

						if (boss.frame >= 310 && boss.frame <= 333)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								192 - 64 * ((boss.frame - 310) / 6), 0, 64, 128, ghCharBossJump, 0xFFFFFFFF
							);
						}

						if (boss.frame > 333)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						break;

					case 2:

						if (boss.frame < 30)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						if (boss.frame >= 30 && boss.frame <= 53)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								64 * ((boss.frame - 30) / 6), 0, 64, 128, ghCharBossJump, 0xFFFFFFFF
							);
						}

						if (boss.frame > 53 && boss.frame < 80)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								192, 0, 64, 128, ghCharBossJump, 0xFFFFFFFF
							);
						}

						if (boss.frame >= 80 && boss.frame < 100)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								128, 0, 64, 128, ghCharBossEnemySky, 0xFFFFFFFF
							);
						}

						if (boss.frame >= 100 && boss.frame < 127)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								192, 0, 64, 128, ghCharBossRoof, 0xFFFFFFFF
							);
						}

						if (boss.frame >= 127 && boss.frame <= 150)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								192 - 64 * ((boss.frame - 127) / 6), 0, 64, 128, ghCharBossRoof, 0xFFFFFFFF
							);
						}

						if (boss.frame > 150)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						break;

					case 3:

						if (boss.frame < 30)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						if (boss.frame >= 30 && boss.frame <= 77)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								64 * ((boss.frame - 30) / 12), 0, 64, 128, ghCharBossEnemyGround, 0xFFFFFFFF
							);
						}

						if (boss.frame > 77)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						break;

					case 4:

						if (boss.frame < 30)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						if (boss.frame >= 30 && boss.frame <= 80)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								64 * ((boss.frame - 30) / 13), 0, 64, 128, ghCharBossEnemySky, 0xFFFFFFFF
							);
						}

						if (boss.frame >= 80 && boss.frame <= 240)
						{
							Novice::DrawQuad
							(
								static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
								static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
								static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
								static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
								192, 0, 64, 128, ghCharBossEnemySky, 0xFFFFFFFF
							);
						}

						if (boss.frame > 240)
						{
							if (boss.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							} else if (boss.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
									static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
									static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
									static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
									0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
								);
							}
						}

						break;
					}
				} else
				{
					if (boss.directionNo == DIRECTION_LEFT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
							static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
							static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
							static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
							0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
						);
					} else if (boss.directionNo == DIRECTION_RIGHT)
					{
						Novice::DrawQuad
						(
							static_cast<int>(boss.pos.screen.rightTop.x), static_cast<int>(boss.pos.screen.rightTop.y),
							static_cast<int>(boss.pos.screen.leftTop.x), static_cast<int>(boss.pos.screen.leftTop.y),
							static_cast<int>(boss.pos.screen.rightBottom.x), static_cast<int>(boss.pos.screen.rightBottom.y),
							static_cast<int>(boss.pos.screen.leftBottom.x), static_cast<int>(boss.pos.screen.leftBottom.y),
							0, 0, 64, 128, ghCharBoss, 0xFFFFFFFF
						);
					}
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
									64 * ((enemy[i].frame % 24) / 8), 0, 64, 64, ghCharEnemyGround, 0xFFFFFFFF
								);
							} else if (enemy[i].directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
									static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
									static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
									static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
									64 * ((enemy[i].frame % 24) / 8), 0, 64, 64, ghCharEnemyGround, 0xFFFFFFFF
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
								64 * ((enemy[i].frame % 24) / 8), 0, 64, 64, ghCharEnemyFlying, 0xFFFFFFFF
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
								64 * ((enemy[i].frame % 24) / 8), 0, 64, 64, ghCharEnemyGround, 0xFFFFFFFF
							);
						} else if (enemy[i].directionNo == DIRECTION_RIGHT)
						{
							Novice::DrawQuad
							(
								static_cast<int>(enemy[i].pos.screen.rightBottom.x), static_cast<int>(enemy[i].pos.screen.rightBottom.y),
								static_cast<int>(enemy[i].pos.screen.leftBottom.x), static_cast<int>(enemy[i].pos.screen.leftBottom.y),
								static_cast<int>(enemy[i].pos.screen.rightTop.x), static_cast<int>(enemy[i].pos.screen.rightTop.y),
								static_cast<int>(enemy[i].pos.screen.leftTop.x), static_cast<int>(enemy[i].pos.screen.leftTop.y),
								64 * ((enemy[i].frame % 24) / 8), 0, 64, 64, ghCharEnemyGround, 0xFFFFFFFF
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
							64 * ((enemy[i].frame % 24) / 8), 0, 64, 64, ghCharEnemyFlying, 0xFFFFFFFF
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

					switch (bullet[i].type)
					{
					case BULLET_TYPE_RUBBLE:

						Novice::DrawQuad
						(
							static_cast<int>(bullet[i].pos.screen.leftTop.x), static_cast<int>(bullet[i].pos.screen.leftTop.y),
							static_cast<int>(bullet[i].pos.screen.rightTop.x), static_cast<int>(bullet[i].pos.screen.rightTop.y),
							static_cast<int>(bullet[i].pos.screen.leftBottom.x), static_cast<int>(bullet[i].pos.screen.leftBottom.y),
							static_cast<int>(bullet[i].pos.screen.rightBottom.x), static_cast<int>(bullet[i].pos.screen.rightBottom.y),
							0, 0, 612, 612, ghStone, 0xFFFFFFFF
						);

						break;

					case BULLET_TYPE_VIBRATION_LEFT:

						Novice::DrawQuad
						(
							static_cast<int>(bullet[i].pos.screen.rightTop.x), static_cast<int>(bullet[i].pos.screen.rightTop.y),
							static_cast<int>(bullet[i].pos.screen.leftTop.x), static_cast<int>(bullet[i].pos.screen.leftTop.y),
							static_cast<int>(bullet[i].pos.screen.rightBottom.x), static_cast<int>(bullet[i].pos.screen.rightBottom.y),
							static_cast<int>(bullet[i].pos.screen.leftBottom.x), static_cast<int>(bullet[i].pos.screen.leftBottom.y),
							0, 0, 612, 306, ghWave, 0xFFFFFFFF
						);

						break;

					case BULLET_TYPE_VIBRATION_RIGHT:

						Novice::DrawQuad
						(
							static_cast<int>(bullet[i].pos.screen.leftTop.x), static_cast<int>(bullet[i].pos.screen.leftTop.y),
							static_cast<int>(bullet[i].pos.screen.rightTop.x), static_cast<int>(bullet[i].pos.screen.rightTop.y),
							static_cast<int>(bullet[i].pos.screen.leftBottom.x), static_cast<int>(bullet[i].pos.screen.leftBottom.y),
							static_cast<int>(bullet[i].pos.screen.rightBottom.x), static_cast<int>(bullet[i].pos.screen.rightBottom.y),
							0, 0, 612, 306, ghWave, 0xFFFFFFFF
						);

						break;
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
					if (player.vel.x != 0.0f)
					{
						if (player.isBomHave == false)
						{
							if (player.directionNo == DIRECTION_LEFT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
									static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
									static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
									static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
									1924 * (player.frame / 8), 0, 1024, 954, ghCharPlayerRun, 0xFFFFFFFF
								);
							} else if (player.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
									static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
									static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
									static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
									1924 * (player.frame / 8), 0, 1024, 954, ghCharPlayerRun, 0xFFFFFFFF
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
									1924 * (player.frame / 8), 0, 1024, 954, ghCharPlayerBombRun, 0xFFFFFFFF
								);
							} else if (player.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
									static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
									static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
									static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
									1924 * (player.frame / 8), 0, 1024, 954, ghCharPlayerBombRun, 0xFFFFFFFF
								);
							}
						}
					} else
					{
						if (player.isBomHave == false)
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
									0, 0, 1024, 1024, ghCharPlayerBomb, 0xFFFFFFFF
								);
							} else if (player.directionNo == DIRECTION_RIGHT)
							{
								Novice::DrawQuad
								(
									static_cast<int>(player.pos.screen.leftTop.x), static_cast<int>(player.pos.screen.leftTop.y),
									static_cast<int>(player.pos.screen.rightTop.x), static_cast<int>(player.pos.screen.rightTop.y),
									static_cast<int>(player.pos.screen.leftBottom.x), static_cast<int>(player.pos.screen.leftBottom.y),
									static_cast<int>(player.pos.screen.rightBottom.x), static_cast<int>(player.pos.screen.rightBottom.y),
									0, 0, 1024, 1024, ghCharPlayerBomb, 0xFFFFFFFF
								);
							}
						}
					}
				} else
				{

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

			// bgm
			if (!Novice::IsPlayingAudio(sH.pHbattle) || sH.pHbattle == -1) {
				sH.pHbattle = Novice::PlayAudio(sH.battle, 1, 0.3f);
			}

			Novice::StopAudio(sH.pHmenu);

			Novice::ScreenPrintf(100, 100, "%d", player.damage.hp);

			Novice::ScreenPrintf(100, 200, "%f", sH.volumeBomCount);

			break;

		case SCENE_GAME_OVER:

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

			// フレーム
			player.frame = 0;

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


			// 突進のフレーム
			rushFrame = 0;


			/*   パーティクル   */

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


			/*   アイテム   */

			for (int i = 0; i < kItemNum; i++)
			{
				item[i].isShot = false;

				item[i].type = -1;

				item[i].shape.scale = { 0.0f , 0.0f };
				item[i].shape.theta = 0.0f;
				item[i].shape.translate = { 0.0f , 0.0f };

				item[i].pos.world = VertexAffineMatrix(item[i].shape);
				item[i].pos.screen = CoordinateTransformation(item[i].pos.world);

				item[i].vel = { 0.0f , 0.0f };

				item[i].acceleration = { 0.0f , 0.0f };
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				gameState = SCENE_TITLE;

				gameFrame = 0;
			}

			if (!Novice::IsPlayingAudio(sH.pHmenu) || sH.pHmenu == -1) {
				sH.pHmenu = Novice::PlayAudio(sH.menu, 1, 0.3f);
			}

			Novice::StopAudio(sH.pHbattle);

			Novice::DrawSprite(0, 0, ghGameOver, 1, 1, 0.0f, 0xFFFFFFFF);

			break;

		case SCENE_GAME_CLEAR:

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

			// フレーム
			player.frame = 0;

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


			// 突進のフレーム
			rushFrame = 0;


			/*   パーティクル   */

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


			/*   アイテム   */

			for (int i = 0; i < kItemNum; i++)
			{
				item[i].isShot = false;

				item[i].type = -1;

				item[i].shape.scale = { 0.0f , 0.0f };
				item[i].shape.theta = 0.0f;
				item[i].shape.translate = { 0.0f , 0.0f };

				item[i].pos.world = VertexAffineMatrix(item[i].shape);
				item[i].pos.screen = CoordinateTransformation(item[i].pos.world);

				item[i].vel = { 0.0f , 0.0f };

				item[i].acceleration = { 0.0f , 0.0f };
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] || Novice::IsTriggerButton(0, kPadButton10))
			{
				gameState = SCENE_TITLE;

				gameFrame = 600;
			}

			Novice::DrawSprite(0, 0, ghGameClear, 1, 1, 0.0f, 0xFFFFFFFF);

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
