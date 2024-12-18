#include <Novice.h>
#include <time.h>
#include "Structure.h"
#include "Constant.h"
#include "Enumeration.h"
#include "Shapes.h"
#include "Matrix.h"
#include "Player.h"
#include "Bomb.h"
#include "Bullet.h"
#include "shake.h"

void ShakeBG(BackGround* backGround) {

	if (backGround->isShake) {
		backGround->rand.x = rand() % backGround->shakeRange.x - backGround->shakeRange.x / 2;
		backGround->rand.y = rand() % backGround->shakeRange.y - backGround->shakeRange.y / 2;

		backGround->shakeRange.x -= backGround->randSpeed.x;
		backGround->shakeRange.y -= backGround->randSpeed.y;
		if (backGround->shakeRange.x <= 0 || backGround->shakeRange.y <= 0) {
			backGround->rand.x = 0;
			backGround->rand.y = 0;
			backGround->isShake = false;
		}
	}
}

int Shake(int randSize)
{
	int randam = (rand() % (randSize + 1)) - (randSize / 2);

	return randam;
}