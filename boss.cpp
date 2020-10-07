#include "stdafx.h"
#include "boss.h"
#include "player.h"

HRESULT boss::init(int _posX, int _posY)
{
	IMAGEMANAGER->addFrameImage("boss", "resource/boss/bossSprite.bmp", 1500, 1200, 10, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("punch", "resource/boss/0.bmp", 81, 87, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("drill", "resource/boss/1.bmp", 105, 156, 1, 2, true, RGB(255, 0, 255));

	frameX = 0;
	frameY = 5;

	boss.bossState = RESPONE;

	count = timer = 0;
	posX = _posX;
	posY = _posY;

	boss.center.x = posX;
	boss.center.y = posY;
	boss.rc = RectMakeCenter(boss.center.x, boss.center.y, 150, 150);
	boss.angle = 0;

	for (int i = 0; i < 3; i++) {
		block[i].center.x = boss.center.x;
		block[i].center.y = boss.center.y;
		block[i].rc = RectMakeCenter(block[i].center.x, block[i].center.y, 81, 87);
		block[i].angle = 0;
	}

	posPlayer = 5;
	punchCount = 0;
	woodTimer = 0;
	pattern = 0;
	patternCount = 0;

	jumpMotion = false;
	leftCheck = false;
	for (int i = 0; i < 3; i++) {
		punching[i] = false;
	}
	startNiddle = false;
	patternStart = false;

	jumpPattern = false;
	drillPattern = false;
	punchPattern = false;
	niddlePattern = false;
	wallPattern = false;

	niddleAngle = 0.0f;


	//sound

	isPlayDrillSFX = isPlayJumpSFX = isPlayWallSFX = isPlayNiddleSFX = isPlayPunchSFX = false;
	return S_OK;
}

void boss::release()
{
}

void boss::update()
{
	this->bossPattern();

	BOSSMANAGER->update();
	this->animation();
}

void boss::render()
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("boss"), boss.rc.left, boss.rc.top, frameX, frameY);

	if (boss.bossState == DRILL) {
		if (!leftCheck) {
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("drill"), drillBlcok.rc.left, drillBlcok.rc.top, 0, 0);
		}
		else {
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("drill"), drillBlcok.rc.left, drillBlcok.rc.top, 0, 1);
		}
	}

	for (int i = 0; i < 3; i++) {
		if (punching[i]) {
			CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("punch"), block[i].rc.left, block[i].rc.top);
		}
	}

	BOSSMANAGER->render(getMemDC());
}

void boss::animation()
{
	switch (boss.bossState)
	{
	case RESPONE:
		// 기둥 있을떄

		// 기둥 사라졌을때
		frameY = 5;
		count++;
		timer++;
		if (count % 10 == 0) {
			if (frameX < 4) {
				frameX++;
			}
		}
		if (timer > 150) {
			frameX = 0;
			count = 0;
			timer = 0;
			boss.bossState = BOSSIDLE;
		}
		break;
	case BOSSIDLE:
		frameX = frameY = 0;
		isPlayDrillSFX = isPlayJumpSFX = isPlayWallSFX = isPlayNiddleSFX = isPlayPunchSFX = false;
		break;
	case JUMP:
		if (!jumpMotion) {
			count++;
			if (count % 10 == 0) {
				if (frameX <= 2) {
					count = 0;
					frameX = 2;
					timer++;
					if (timer > 5) {
						jumpMotion = true;
						timer = 0;
						frameX = 0;
						count = 0;
						if (!isPlayJumpSFX)
						{
							SOUNDMANAGER->play("bossJumpSFX", false);
							SOUNDMANAGER->play("bossJumpVO", false);
							isPlayJumpSFX = true;
						}
					}
				}
				if (frameX > 2) {
					frameX--;
				}
			}
		}
		else {
			frameY = 7;
			count++;
			if (count % 20 == 0) {
				if (frameX >= 2) {
					count = 0;
					frameX = 2;
					timer++;
					if (timer > 2) {
						timer = 0;
						jumpMotion = false;
						isPlayJumpSFX = false;
						boss.bossState = BOSSIDLE;
					}
				}
				else {
					frameX++;
				}
			}
		}
		break;
	case DRILL:
		if (!isPlayDrillSFX)
		{
			SOUNDMANAGER->play("bossDrillSFX", false);
			SOUNDMANAGER->play("bossDrillVO", false);
			isPlayDrillSFX = true;
		}
		if (leftCheck) {
			frameX = 9;
			frameY = 2;
			timer++;
			if (timer > 30) {
				frameX = 8;
			}
		}
		else {
			frameX = 0;
			frameY = 2;
			timer++;
			if (timer > 30) {
				frameX = 1;
			}
		}
		break;
	case PUNCH:
		if (!isPlayPunchSFX)
		{
			SOUNDMANAGER->play("bossPunchSFX", false);
			SOUNDMANAGER->play("bossPunchVO", false);
			isPlayPunchSFX = true;
		}
		if (punchCount == 0) {
			switch (posPlayer)
			{
			case 1:
				frameX = 0;
				frameY = 3;
				timer++;
				if (timer > 30) {
					frameX = 1;
					punching[0] = true;
				}
				if (timer > 60) {
					punchCount++;
					leftCheck = true;
					timer = 0;
					this->bossPlayerAngle();
					block[1].angle = boss.angle;
					punching[1] = true;
				}
				break;
			case 2:
				frameX = 9;
				frameY = 2;
				timer++;
				if (timer > 30) {
					frameX = 8;
					punching[0] = true;
				}
				if (timer > 60) {
					punchCount++;
					leftCheck = false;
					timer = 0;
					this->bossPlayerAngle();
					block[1].angle = boss.angle;
					punching[1] = true;
				}
				break;
			case 3:
				frameX = 9;
				frameY = 5;
				timer++;
				if (timer > 30) {
					frameX = 6;
					punching[0] = true;
				}
				if (timer > 60) {
					punchCount++;
					leftCheck = false;
					timer = 0;
					this->bossPlayerAngle();
					block[1].angle = boss.angle;
					punching[1] = true;
				}
				break;
			case 4:
				frameX = 0;
				frameY = 2;
				timer++;
				if (timer > 30) {
					frameX = 1;
					punching[0] = true;
				}
				if (timer > 60) {
					punchCount++;
					leftCheck = true;
					timer = 0;
					this->bossPlayerAngle();
					block[1].angle = boss.angle;
					punching[1] = true;
				}
				break;
			}
		}
		else if (punchCount == 1) {
			switch (posPlayer)
			{
			case 1:
				if (leftCheck) {
					frameX = 0;
					frameY = 3;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = false;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				else {
					frameX = 1;
					frameY = 3;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = true;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				break;
			case 2:
				if (leftCheck) {
					frameX = 8;
					frameY = 2;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = false;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				else {
					frameX = 7;
					frameY = 2;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = true;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				break;
			case 3:
				if (leftCheck) {
					frameX = 9;
					frameY = 6;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = false;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				else {
					frameX = 3;
					frameY = 1;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = true;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				break;
			case 4:
				if (leftCheck) {
					frameX = 6;
					frameY = 6;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = false;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				else {
					frameX = 1;
					frameY = 2;
					timer++;
					if (timer > 30) {
						punchCount++;
						timer = 0;
						leftCheck = true;
						this->bossPlayerAngle();
						block[2].angle = boss.angle;
						punching[2] = true;
					}
				}
				break;
			}
		}
		else if (punchCount == 2) {
			switch (posPlayer)
			{
			case 1:
				if (leftCheck) {
					frameX = 0;
					frameY = 3;
				}
				else {
					frameX = 1;
					frameY = 3;
				}
				timer++;
				if (timer > 40) {
					punchCount = 0;
					timer = 0;
					leftCheck = false;
					boss.bossState = BOSSIDLE;
					isPlayPunchSFX = false;
					punchPattern = false;
					patternStart = false;
				}
				break;
			case 2:
				if (leftCheck) {
					frameX = 8;
					frameY = 2;
				}
				else {
					frameX = 7;
					frameY = 2;
				}
				timer++;
				if (timer > 40) {
					punchCount = 0;
					timer = 0;
					leftCheck = false;
					boss.bossState = BOSSIDLE;
					isPlayPunchSFX = false;
					punchPattern = false;
					patternStart = false;
				}
				break;
			case 3:
				if (leftCheck) {
					frameX = 9;
					frameY = 6;
				}
				else {
					frameX = 3;
					frameY = 1;
				}
				timer++;
				if (timer > 40) {
					punchCount = 0;
					timer = 0;
					leftCheck = false;
					boss.bossState = BOSSIDLE;
					isPlayPunchSFX = false;
					punchPattern = false;
					patternStart = false;
				}
				break;
			case 4:
				if (leftCheck) {
					frameX = 6;
					frameY = 6;
				}
				else {
					frameX = 1;
					frameY = 2;
				}
				timer++;
				if (timer > 40) {
					punchCount = 0;
					timer = 0;
					leftCheck = false;
					isPlayPunchSFX = false;
					boss.bossState = BOSSIDLE;
					punchPattern = false;
					patternStart = false;
				}
				break;
			}
		}
		break;
	case NIDDLE:
		if (!isPlayNiddleSFX)
		{
			SOUNDMANAGER->play("bossNiddleSFX", false);
			SOUNDMANAGER->play("bossNiddleVO", false);
			isPlayNiddleSFX = true;
		}
		if (!jumpMotion) {
			frameY = 0;
			count++;
			if (count % 10 == 0) {
				if (frameX < 2) {
					count = 0;
					timer = 0;
					frameX = 0;
					jumpMotion = true;
				}
				if (!jumpMotion) {
					frameX--;
				}
			}
		}
		else {
			frameY = 7;
			count++;
			if (count % 20 == 0) {
				if (frameX >= 2) {
					count = 0;
					frameX = 2;
				}
				else {
					frameX++;
				}
			}
		}
		break;
	case WALL:
		if (!isPlayWallSFX)
		{
			SOUNDMANAGER->play("bossWallSFX", false);
			SOUNDMANAGER->play("bossWallVO", false);
			isPlayWallSFX = true;
		}
		frameX = 3;
		frameY = 0;
		timer++;
		if (timer > 50) {
			frameX = 2;
		}
		break;
	}
}
 //플레이어 위치, 보스 범위 조정 필요
void boss::bossPlayerAngle()
{
	boss.center.x = boss.rc.left + 75;
	boss.center.y = boss.rc.top + 75;
	posX = _player->getX();
	posY = _player->getY();
	boss.angle = getAngle(boss.center.x, boss.center.y, posX, posY);
	if (boss.angle >= 1.0f && boss.angle < 2.5f) {
		posPlayer = 1;
	}
	else if (boss.angle >= 2.5f && boss.angle < 4.0f) {
		posPlayer = 2;
	}
	else if (boss.angle >= 4.0f && boss.angle < 5.5f) {
		posPlayer = 3;
	}
	else if (boss.angle >= 5.5f || boss.angle < 1.0f) {
		posPlayer = 4;
	}
}

void boss::jump(int patternType)
{
	if (patternType == 1 && !jumpPattern) {
		jumpPattern = true;
		frameX = 4;
		frameY = 0;
		jumpMotion = false;
		boss.bossState = JUMP;
		posX = _player->getX();
		posY = _player->getRect().bottom - WINSIZEY;
	}

	if (boss.bossState == JUMP && jumpMotion) {
		if (frameX == 1) {
			if (boss.rc.bottom < 0) {
				boss.rc = RectMakeCenter(posX, posY, 150, 150);
			}
			else {
				boss.rc.top -= 50;
				boss.rc.bottom -= 50;
			}
		}
		else if (frameX == 2) {
			if (boss.rc.bottom <= posY + WINSIZEY) {
				boss.rc.top += 50;
				boss.rc.bottom += 50;
			}
			else {
				CAMERAMANAGER->Shake(30, 30, 15);
				boss.center.x = boss.rc.left + 75;
				boss.center.y = boss.rc.top + 75;
				patternStart = false;
				jumpPattern = false;
			}
		}
	}
}

void boss::drill(int patternType)
{
	if (patternType == 2 && !drillPattern) {
		boss.bossState = DRILL;
		count = 0;
		timer = 0;
		boss.center.x = boss.rc.left + 75;
		boss.center.y = boss.rc.top + 75;
		posX = _player->getX();
		posY = _player->getY();
		boss.angle = getAngle(boss.center.x, boss.center.y, posX, posY);
		if (boss.rc.right < _player->getRect().left) {
			drillBlcok.rc = RectMakeCenter(boss.center.x + 50, boss.center.y + 20, 100, 80);
			leftCheck = false;
		}
		else {
			leftCheck = true;
			drillBlcok.rc = RectMakeCenter(boss.center.x - 50, boss.center.y + 20, 100, 80);
		}
		drillPattern = true;
	}

	if (boss.bossState == DRILL && timer > 30) {
		if (boss.rc.left > 100 && boss.rc.right < WINSIZEX + 100 && boss.rc.top > 100 && boss.rc.bottom < WINSIZEY) {
			boss.center.x += cosf(boss.angle) * 30;
			boss.center.y += -sinf(boss.angle) * 30;
			boss.rc = RectMakeCenter(boss.center.x, boss.center.y, 150, 150);
			if (leftCheck) {
				drillBlcok.rc = RectMakeCenter(boss.center.x - 75, boss.center.y + 20, 100, 80);
			}
			else {
				drillBlcok.rc = RectMakeCenter(boss.center.x + 75, boss.center.y + 20, 100, 80);
			}
		}
		else {
			count++;
			if (count > 50) {
				if (boss.rc.left <= 100) {
					boss.center.x += 30;
				}
				else if (boss.rc.right >= WINSIZEX + 100) {
					boss.center.x -= 30;
				}
				else if (boss.rc.top <= 100) {
					boss.center.y += 30;
				}
				else if (boss.rc.bottom >= WINSIZEY + 100) {
					boss.center.y -= 30;
				}
				boss.rc = RectMakeCenter(boss.center.x, boss.center.y, 150, 150);
				boss.bossState = BOSSIDLE;
				drillPattern = false;
				patternStart = false;
				timer = 0;
				count = 0;
			}
		}
	}
}

void boss::punch(int patternType)
{
	if (patternType == 3 && !punchPattern) {
		boss.bossState = PUNCH;
		count = 0;
		timer = 0;
		this->bossPlayerAngle();
		for (int i = 0; i < 3; i++) {
			block[i].center.x = boss.center.x;
			block[i].center.y = boss.center.y;
			block[i].angle = boss.angle;
			block[i].rc = RectMakeCenter(block[i].center.x, block[i].center.y, 100, 100);
		}
		punchPattern = true;
	}

	if (boss.bossState == PUNCH) {
		for (int i = 0; i < 3; i++) {
			if (punching[i]) {
				block[i].center.x += cosf(block[i].angle) * 35;
				block[i].center.y += -sinf(block[i].angle) * 35;
				block[i].rc = RectMakeCenter(block[i].center.x, block[i].center.y, 100, 100);
				CAMERAMANAGER->Shake(10, 10, 2);
			}
			if (block[i].rc.left > WINSIZEX + 150|| block[i].rc.right < 100 || block[i].rc.top > WINSIZEY || block[i].rc.bottom < 150) {
				punching[i] = false;
			}
		}
	}
}

void boss::niddle(int patternType)
{
	if (patternType == 4 && !niddlePattern) {
		boss.bossState = NIDDLE;
		jumpMotion = false;
		timer = 0;
		frameX = 4;
		niddlePattern = true;
	}

	if (boss.bossState == NIDDLE && jumpMotion) {
		if (frameX == 1) {
			if (boss.rc.bottom < 0) {
				boss.rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 - WINSIZEY, 150, 150);
			}
			else {
				boss.rc.top -= 50;
				boss.rc.bottom -= 50;
			}
		}
		else if (frameX == 2 && !startNiddle) {
			if (boss.rc.bottom <= WINSIZEY / 2 + 75) {
				boss.rc.top += 50;
				boss.rc.bottom += 50;
			}
			else {
				for (int i = 0; i < 5; i++) {
					//niddleBlock[i].angle = PI_2 / 5 * i * 72 + 18;
					niddleBlock[i].angle = (18 + (i * 72)) * PI / 180;
					niddleBlock[i].center.x = cosf(niddleBlock[i].angle) * 130 + WINSIZEX / 2;
					niddleBlock[i].center.y = -sinf(niddleBlock[i].angle) * 130 + WINSIZEY / 2;
					niddleBlock[i].rc = RectMakeCenter(niddleBlock[i].center.x, niddleBlock[i].center.y, 20, 20);
					niddleBlock[i].isNear = abs((niddleBlock[i].angle + PI) > PI * 2 ? niddleBlock[i].angle - PI : niddleBlock[i].angle + PI - getAngle(niddleBlock[i].center.x, niddleBlock[i].center.y, _player->getX(), _player->getY())) < PI ? true : false;
					startNiddle = true;
				}
				boss.center.x = boss.rc.left + 75;
				boss.center.y = boss.rc.top + 75;
			}
		}
	}

	if (startNiddle) {
		timer++;
		for (int i = 0; i < 5; i++) {
			niddleAngle = getAngle(niddleBlock[i].center.x, niddleBlock[i].center.y, _player->getX(), _player->getY());

			if (timer % 5 == 0) {
				BOSSMANAGER->init(niddleBlock[i].center.x, niddleBlock[i].center.y, 20, 1);
			}
			if (niddleBlock[i].center.x > _player->getX()) {
				if (niddleBlock[i].angle > niddleAngle) {
					niddleBlock[i].angle -= 0.1f;
				}
				else if (niddleBlock[i].angle < niddleAngle) {
					niddleBlock[i].angle += 0.1f;
				}
			}
			else {
				niddleBlock[i].angle = niddleAngle;
			}

			niddleBlock[i].center.x += cosf(niddleBlock[i].angle) * 5;
			niddleBlock[i].center.y += -sinf(niddleBlock[i].angle) * 5;

			niddleBlock[i].rc = RectMakeCenter(niddleBlock[i].center.x, niddleBlock[i].center.y, 20, 20);

			CAMERAMANAGER->Shake(5, 5, 70);

			if (timer > 400) {
				timer = 0;
				boss.bossState = BOSSIDLE;
				startNiddle = false;
				niddlePattern = false;
				patternStart = false;
			}
		}
	}
}

void boss::middleWood()
{
	tagBlock _wall;
	_wall.center = boss.center;
	_wall.angle = getAngle(_wall.center.x, _wall.center.y, _player->getX(), _player->getY());
	_wall.rc = RectMakeCenter(_wall.center.x, _wall.center.y, 44, 44);
	_wall.type = 0;
	_wall.blockCount = 0;
	wallBlock.push_back(_wall);
}

void boss::wall(int patternType)
{
	if (patternType == 5 && !wallPattern) {
		boss.bossState = WALL;
		count = 0;
		timer = 0;
		if (boss.rc.right < _player->getRect().left) {
			leftCheck = false;
		}
		else {
			leftCheck = true;
		}
		for (int i = -1; i < 2; i++) {
			tagBlock _wall;
			_wall.center = boss.center;
			_wall.angle = getAngle(_wall.center.x, _wall.center.y, _player->getX(), _player->getY()) + i * PI_8;
			_wall.rc = RectMakeCenter(_wall.center.x, _wall.center.y, 44, 44);
			_wall.type = i;
			_wall.blockCount = 0;
			wallBlock.push_back(_wall);
		}
		this->bossPlayerAngle();
		wallPattern = true;
	}

	if (boss.bossState == WALL && timer > 50) {
		count++;
		CAMERAMANAGER->Shake(5, 5, 60);
		if (count > 260) {
			boss.bossState = BOSSIDLE;
			patternStart = false;
			wallPattern = false;
		}
		if (count % 90 == 0) {
			this->middleWood();
		}
		for (int i = 0; i < wallBlock.size(); i++) {
			wallBlock[i].blockCount++;
			if (wallBlock[i].rc.left > 0 && wallBlock[i].rc.right < WINSIZEX + 100 && wallBlock[i].rc.top > 100 && wallBlock[i].rc.bottom < WINSIZEY + 100) {
				if (wallBlock[i].blockCount % 2 == 0 && wallBlock[i].type == -1) {
					switch (posPlayer)
					{
					case 1:
						BOSSMANAGER->init(wallBlock[i].center.x + 30, wallBlock[i].center.y, 240, 0);
						break;
					case 2:
						BOSSMANAGER->init(wallBlock[i].center.x + 130, wallBlock[i].center.y -200, 240, 0);
						break;
					case 3:
						BOSSMANAGER->init(wallBlock[i].center.x - 200, wallBlock[i].center.y - 150, 240, 0);
						break;
					case 4:
						BOSSMANAGER->init(wallBlock[i].center.x -130, wallBlock[i].center.y, 240, 0);
						break;
					}
				}
				if (wallBlock[i].blockCount % 2 == 0 && wallBlock[i].type == 0) {
					BOSSMANAGER->init(wallBlock[i].center.x + (leftCheck ? -50 : 50), wallBlock[i].center.y - 110, 80, 0);
				}
				if (wallBlock[i].blockCount % 2 == 0 && wallBlock[i].type == 1) {
					switch (posPlayer)
					{
					case 1:
						BOSSMANAGER->init(wallBlock[i].center.x - 170, wallBlock[i].center.y, 240, 0);
						break;
					case 2:
						BOSSMANAGER->init(wallBlock[i].center.x + 130, wallBlock[i].center.y, 240, 0);
						break;
					case 3:
						BOSSMANAGER->init(wallBlock[i].center.x + 50, wallBlock[i].center.y - 150, 240, 0);
						break;
					case 4:
						BOSSMANAGER->init(wallBlock[i].center.x - 130, wallBlock[i].center.y - 200, 240, 0);
						break;
					}
				}
				wallBlock[i].center.x += cosf(wallBlock[i].angle) * 55;
				wallBlock[i].center.y += -sinf(wallBlock[i].angle) * 55;
				wallBlock[i].rc = RectMakeCenter(wallBlock[i].center.x, wallBlock[i].center.y, 48, 44);
			}
		}
	}
}

void boss::bossPattern()
{
	if (boss.bossState == BOSSIDLE) {
		pattern = RANDOM->range(5) + 1;
		patternStart = true;
		patternCount++;
	}
	if (patternStart && patternCount < 4) {
		this->jump(pattern);
		this->drill(pattern);
		this->punch(pattern);
		this->niddle(pattern);
		this->wall(pattern);
	}
	if (patternCount > 4) {
		timer++;
		cout << timer << endl;
		if (timer > 200) {
			timer = 0;
			patternCount = 0;
		}
	}
}
