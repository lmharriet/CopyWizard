#include "stdafx.h"
#include "finalBoss.h"
#include "player.h"

HRESULT finalBoss::init(int _posX, int _posY)
{
	IMAGEMANAGER->addFrameImage("finalboss", "resource/boss/master_sura.bmp", 2000, 800, 20, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bossice", "resource/player/iceSpear.bmp", 96, 96, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("bossframebar", "resource/boss/bossProgressBarFrame.bmp", 452, 64, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bosshpbar", "resource/boss/bossHpBar.bmp", 356, 28, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bosshpbackBar", "resource/boss/bossHpBackBar.bmp", 356, 28, true, RGB(255, 0, 255));

	boss.center = { _posX, _posY };
	boss.angle = 0;
	boss.bossHp = 2000;
	boss.isHit = false;
	boss.isCol = false;
	boss.dashDintance = 400;
	boss.bossSkill = SKILL2;
	boss.rc = RectMakeCenter(boss.center.x, boss.center.y, 100, 100);
	boss.bossState = FINALBOSSSPONE;

	dashRc = RectMakeCenter(0, 0, 30, 30);

	frameX = 0;
	frameY = 0;
	dashX = 0;
	dashY = 0;
	posDashRc = 0;
	dashCount = 6;
	posPlayer = 0;
	hitTimer = 0;
	skillNum = RANDOM->range(5);

	dashAngle = 0;

	timer = 0;
	count = 0;
	patternCount = 0;

	changStage = false;
	leftCheck = false;
	setDashrc = false;
	wallPattern = false;
	blazePattern = false;
	thunderPattern = false;
	patternStart = false;
	windPattern = false;
	icePattern = false;

	isHit = false;
	bossRespone = false;

	return S_OK;
}

void finalBoss::release()
{
}

void finalBoss::update()
{
	this->animation();
	this->dashColPlayer();
	this->useSkill();
	this->die();
	this->colCheck();
	BOSSMANAGER->update();
}

void finalBoss::render()
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("finalboss"), boss.rc.left, boss.rc.top, frameX, frameY);

	BOSSMANAGER->render(getMemDC());
	EFFECT->AfterimageRender(getMemDC());

	if (icePattern) {
		for (int i = 0; i < iceBlock.size(); i++) {
			CAMERAMANAGER->RotateRender(getMemDC(), IMAGEMANAGER->findImage("bossice"), iceBlock[i]->center.x, iceBlock[i]->center.y, boss.angle);
		}
	}
}

void finalBoss::animation()
{
	switch (boss.bossState)
	{
	case FINALBOSSSPONE:
		if (!changStage) {
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 19) {
					frameX = 19;
					timer++;
					if (timer > 10) {
						changStage = true;
						frameX = 0;
						frameY = 6;
						timer = 0;
						count = 0;
					}
				}
			}
		}
		else {
			count++;
			if (count % 5 == 0) {
				if (frameX == 0) {
					timer++;
					if (timer > 5) {
						frameX++;
						timer = 0;
					}
				}
				else {
					frameX++;
				}
				if (frameX > 5) {
					timer++;
					if (timer > 10) {
						boss.bossState = FINALBOSSIDLE;
						count = 0;
						timer = 0;
					}
				}
			}
		}
		break;
	case FINALBOSSIDLE:
		frameX = 19;
		frameY = 0;
		break;
	case FINALBOSSDASH:
		switch (posDashRc)
		{
		case 1:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 6) {
					frameX = 6;
				}
			}
			break;
		case 2:
			count++;
			if (count % 5 == 0) {
				frameX--;
				if (frameX < 5) {
					frameX = 5;
				}
			}
			break;
		case 3:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 11) {
					frameX = 11;
				}
			}
			break;
		case 4:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 6) {
					frameX = 6;
				}
			}
			break;
		}
		break;
	case FINALBOSSSKILL:
		switch (boss.bossSkill)
		{
		case SKILL1:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 5) {
					frameX = 5;
					timer++;
					if (timer > 10) {
						boss.bossState = FINALBOSSIDLE;
						setDashrc = false;
						wallPattern = false;
						patternStart = false;
						dashCount = RANDOM->range(4, 7);
						count = 0;
						timer = 0;
					}
				}
			}
			break;
		case SKILL2:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 5) {
					frameX = 5;
					timer++;
					if (timer > 10) {
						boss.bossState = FINALBOSSIDLE;
						setDashrc = false;
						blazePattern = false;
						patternStart = false;
						dashCount = RANDOM->range(4, 7);
						count = 0;
						timer = 0;
					}
				}
			}
			break;
		case SKILL3:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 5) {
					frameX = 5;
					timer++;
					if (timer > 10) {
						boss.bossState = FINALBOSSIDLE;
						setDashrc = false;
						thunderPattern = false;
						patternStart = false;
						dashCount = RANDOM->range(4, 7);
						count = 0;
						timer = 0;
					}
				}
			}
			break;
		case SKILL4:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 5) {
					frameX = 5;
					timer++;
					if (timer > 5) {
						boss.bossState = FINALBOSSIDLE;
						setDashrc = false;
						windPattern = false;
						patternStart = false;
						dashCount = RANDOM->range(4, 7);
						count = 0;
						timer = 0;
					}
				}
			}
			break;
		case SKILL5:
			count++;
			if (count % 5 == 0) {
				frameX++;
				if (frameX > 5) {
					frameX = 5;
					timer++;
					if (timer > 5) {
						boss.bossState = FINALBOSSIDLE;
						setDashrc = false;
						icePattern = false;
						patternStart = false;
						dashCount = RANDOM->range(4, 7);
						count = 0;
						timer = 0;
					}
				}
			}
			break;
		}
		break;
	case FINALBOSSDAMAGED:
		frameY = 7;
		if (!bossRespone) {
			count++;
			if (count % 10 == 0) {
				frameX++;
				if (frameX > 7) {
					frameX = 7;
					timer++;
					if (timer > 15) {
						bossRespone = true;
						timer = 0;
						count = 0;
					}
				}
			}
		}
		else {
			count++;
			if (count % 10 == 0) {
				frameX++;
				if (frameX > 10) {
					boss.bossState = FINALBOSSIDLE;
					dashCount = RANDOM->range(4, 7);
					count = 0;
					patternCount = 0;
					bossRespone = false;
				}
			}
		}
		break;
	case FINALBOSSDIE:
		frameY = 7;
		count++;
		if (count % 10 == 0) {
			frameX++;
			if (frameX > 7) {
				frameX = 7;
			}
		}
		break;
	}
}

void finalBoss::updateDashRect()
{
	if (boss.bossState == FINALBOSSIDLE && !setDashrc && patternCount != 4) {
		this->getPosDashRc();

		if (getDistance(boss.center.x, boss.center.y, _player->getX(), _player->getY()) < boss.dashDintance) {
			if (dashCount != 0) {
				boss.dashDintance = 400;
			}
			else {
				boss.dashDintance = getDistance(boss.center.x, boss.center.y, _player->getX(), _player->getY());
				dashCount = 6;
			}
		}
		else {
			boss.dashDintance = 400;
		}
		dashX = boss.center.x + cosf(dashAngle) * boss.dashDintance;
		dashY = boss.center.y + -sinf(dashAngle) * boss.dashDintance;

		dashRc = RectMakeCenter(dashX, dashY, 50, 50);

		switch (posDashRc)
		{
		case 1:
			frameX = 1;
			frameY = 5;
			count = 0;
			break;
		case 2:
			frameX = 11;
			frameY = 2;
			count = 0;
			break;
		case 3:
			frameX = 7;
			frameY = 1;
			count = 0;
			break;
		case 4:
			frameX = 0;
			frameY = 1;
			count = 0;
			break;
		}
		boss.bossState = FINALBOSSDASH;
		setDashrc = true;
	}
	if (patternCount == 4) {
		boss.bossState = FINALBOSSDAMAGED;
	}
}

void finalBoss::dashPattern()
{
	RECT temp;

	this->updateDashRect();

	if (boss.bossState == FINALBOSSDASH) {
		boss.center.x += cosf(dashAngle) * 20;
		boss.center.y += -sinf(dashAngle) * 20;
		boss.rc = RectMakeCenter(boss.center.x, boss.center.y, 100, 100);
		EFFECT->AfterimageEft("finalboss", boss.center, { frameX, frameY }, 2);
	}

	if (IntersectRect(&temp, &dashRc, &boss.rc)) {
		boss.isCol = true;
		setDashrc = false;
		boss.bossState = FINALBOSSIDLE;
		dashCount--;
	}
	
}

void finalBoss::dashColPlayer()
{
	if (((getDistance(boss.center.x, boss.center.y, _player->getX(), _player->getY()) < 150 && dashCount < 3) || dashCount == 0) && boss.bossState != FINALBOSSSKILL) {
		//스킬발동
		patternStart = true;

		boss.bossState = FINALBOSSSKILL;
	}
	if (boss.bossState != FINALBOSSSKILL) {
		this->dashPattern();
	}
}

void finalBoss::getPosDashRc()
{
	dashAngle = getAngle(boss.center.x, boss.center.y, _player->getX(), _player->getY());

	if (dashAngle >= 1.0f && dashAngle < 2.5f) {
		posDashRc = 1;
	}
	else if (dashAngle >= 2.5f && dashAngle < 4.0f) {
		posDashRc = 2;
	}
	else if (dashAngle >= 4.0f && dashAngle < 5.5f) {
		posDashRc = 3;
	}
	else if (dashAngle >= 5.5f || dashAngle < 1.0f) {
		posDashRc = 4;
	}
}

void finalBoss::useSkill()
{
	if (patternStart) {
		skillNum = RANDOM->range(5);
		//skillNum = 1;
		patternCount++;
		switch (skillNum)
		{
		case 0:
			boss.bossSkill = SKILL1;
			break;
		case 1:
			boss.bossSkill = SKILL2;
			break;
		case 2:
			boss.bossSkill = SKILL3;
			break;
		case 3:
			boss.bossSkill = SKILL4;
			break;
		case 4:
			boss.bossSkill = SKILL5;
			break;
		}
		patternStart = false;
	}
	if (boss.bossState == FINALBOSSSKILL) {
		switch (boss.bossSkill)
		{
		case SKILL1:
			this->wall();
			break;
		case SKILL2:
			this->blaze();
			break;
		case SKILL3:
			this->thunder();
			break;
		case SKILL4:
			this->wind();
			break;
		case SKILL5:
			this->ice();
			break;
		}
	}
}

void finalBoss::posPlayerAngle()
{
	boss.angle = getAngle(boss.center.x, boss.center.y, _player->getX(), _player->getY());
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

void finalBoss::wall()
{
	if (!wallPattern) {
		frameX = 0;
		frameY = 6;
		wallBlock.clear();
		for (int i = 0; i < 8; i++) {
			skillBlock* _wall = new skillBlock;
			_wall->angle = (i * 45) * PI / 180;;
			_wall->blockCount = 0;
			_wall->rc = RectMakeCenter(boss.center.x, boss.center.y, 44, 44);
			_wall->center.x = cosf(_wall->angle) * 30 + boss.center.x - 50;
			_wall->center.y = -sinf(_wall->angle) * 30 + boss.center.y - 150;
			_wall->rc = RectMakeCenter(_wall->center.x, _wall->center.y, 20, 20);
			wallBlock.push_back(_wall);
		}
		wallPattern = true;
	}
	else if (wallPattern && frameX == 5) {
		for (int i = 0; i < wallBlock.size(); i++) {
			if (wallBlock[i]->rc.left > 0 && wallBlock[i]->rc.right < 2048 && wallBlock[i]->rc.top > 0 && wallBlock[i]->rc.bottom < 2048) {
				wallBlock[i]->blockCount++;
				if (wallBlock[i]->blockCount % 5 == 0) {
					BOSSMANAGER->init(wallBlock[i]->center.x, wallBlock[i]->center.y, 40, 0);
				}
				wallBlock[i]->center.x += cosf(wallBlock[i]->angle) * 15;
				wallBlock[i]->center.y += -sinf(wallBlock[i]->angle) * 15;
				wallBlock[i]->rc = RectMakeCenter(wallBlock[i]->center.x, wallBlock[i]->center.y, 48, 44);
			}
		}
	}
}

void finalBoss::blaze()
{
	if (!blazePattern) {
		frameX = 0;
		frameY = 6;
		blazeBlock.clear();
		for (int i = 0; i < 18; i++) {
			skillBlock* _blaze = new skillBlock;
			_blaze->angle = (i * 20) * PI / 180;
			_blaze->blockCount = 0;
			_blaze->isHit = false;
			_blaze->rc = RectMakeCenter(boss.center.x, boss.center.y, 44, 44);
			_blaze->center.x = cosf(_blaze->angle) * 100 + boss.center.x - 50;
			_blaze->center.y = -sinf(_blaze->angle) * 100 + boss.center.y - 100;
			_blaze->rc = RectMakeCenter(_blaze->center.x, _blaze->center.y, 20, 20);
			blazeBlock.push_back(_blaze);
		}
		//this->posPlayerAngle();
		blazePattern = true;
	}
	else if (blazePattern && frameX == 5) {
		for (int i = 0; i < blazeBlock.size(); i++) {
			if (blazeBlock[i]->rc.left > 0 && blazeBlock[i]->rc.right < 2048 && blazeBlock[i]->rc.top > 0 && blazeBlock[i]->rc.bottom < 2048) {
				blazeBlock[i]->blockCount++;
				if (blazeBlock[i]->blockCount % 2 == 0) {
					BOSSMANAGER->init(blazeBlock[i]->center.x, blazeBlock[i]->center.y, 10, 4);
				}
				blazeBlock[i]->center.x += cosf(blazeBlock[i]->angle) * 10;
				blazeBlock[i]->center.y += -sinf(blazeBlock[i]->angle) * 10;
				blazeBlock[i]->rc = RectMakeCenter(blazeBlock[i]->center.x, blazeBlock[i]->center.y, 48, 44);
			}
		}
	}
}

void finalBoss::thunder()
{
	if (!thunderPattern) {
		frameX = 0;
		frameY = 6;
		thunderBlock.clear();
		for (int i = 0; i < 3; i++) {
			skillBlock* _thunder = new skillBlock;
			_thunder->angle = (i * 120) * PI / 180;
			_thunder->blockCount = 0;
			_thunder->rc = RectMakeCenter(boss.center.x, boss.center.y, 700, 700);
			_thunder->center.x = cosf(_thunder->angle) * 200 + boss.center.x - 50;
			_thunder->center.y = -sinf(_thunder->angle) * 200 + boss.center.y - 100;
			_thunder->rc = RectMakeCenter(_thunder->center.x, _thunder->center.y, 20, 20);
			thunderBlock.push_back(_thunder);
		}
		thunderPattern = true;
	}
	else if (thunderPattern && frameX == 5) {
		for (int i = 0; i < thunderBlock.size(); i++) {
			thunderBlock[i]->blockCount++;
			if (thunderBlock[i]->blockCount % 3 == 0) {
				BOSSMANAGER->init(thunderBlock[i]->center.x - 300, thunderBlock[i]->center.y - 250, 4, 5);
			}
		}
	}
}

void finalBoss::wind()
{
	if (!windPattern) {
		frameX = 0;
		frameY = 6;
		this->posPlayerAngle();
		windBlock.clear();
		skillBlock* _wind = new skillBlock;
		_wind->angle = boss.angle;
		_wind->blockCount = 0;
		_wind->rc = RectMakeCenter(boss.center.x, boss.center.y, 700, 700);
		_wind->center.x = cosf(_wind->angle) + boss.center.x - 50;
		_wind->center.y = -sinf(_wind->angle) + boss.center.y - 100;
		_wind->rc = RectMakeCenter(_wind->center.x, _wind->center.y, 20, 20);
		windBlock.push_back(_wind);
		windPattern = true;
	}
	else if (windPattern && frameX == 5) {
		for (int i = 0; i < windBlock.size(); i++) {
			windBlock[i]->blockCount++;
			if (windBlock[i]->blockCount % 5 == 0) {
				BOSSMANAGER->init(windBlock[i]->center.x - 100, windBlock[i]->center.y, 20, 6);
			}
			windBlock[i]->center.x += cosf(windBlock[i]->angle) * 30;
			windBlock[i]->center.y += -sinf(windBlock[i]->angle) * 30;
			windBlock[i]->rc = RectMakeCenter(windBlock[i]->center.x, windBlock[i]->center.y, 149, 116);
		}
	}
}

void finalBoss::ice()
{
	if (!icePattern) {
		frameX = 0;
		frameY = 6;
		iceBlock.clear();
		for (int i = 0; i < 5; i++) {
			skillBlock* _ice = new skillBlock;
			_ice->angle = (i * 72) * PI / 180;
			_ice->blockCount = 0;
			_ice->isHit = false;
			_ice->rc = RectMakeCenter(boss.center.x, boss.center.y, 96, 96);
			_ice->center.x = cosf(_ice->angle) * 100 + boss.center.x - 50;
			_ice->center.y = -sinf(_ice->angle) * 100 + boss.center.y - 100;
			_ice->rc = RectMakeCenter(_ice->center.x, _ice->center.y, 20, 20);
			iceBlock.push_back(_ice);
		}
		icePattern = true;
	}
	else {
		for (int i = 0; i < iceBlock.size(); i++) {
			if (frameX != 5) {
				this->posPlayerAngle();
			}
			else {
				iceBlock[i]->center.x += cosf(boss.angle) * 40;
				iceBlock[i]->center.y += -sinf(boss.angle) * 40;
				iceBlock[i]->rc = RectMakeCenter(iceBlock[i]->center.x, iceBlock[i]->center.y, 96, 96);
			}
		}
	}
}

void finalBoss::die()
{
	if (boss.bossHp <= 0) {
		boss.bossHp = 0;
		boss.bossState = FINALBOSSDIE;
	}
}

void finalBoss::colCheck()
{
	RECT temp;
	for (int i = 0; i < BOSSMANAGER->getVector().size(); i++) {
		if (IntersectRect(&temp, &_player->getRect(), &BOSSMANAGER->getVector()[i]->getRect())) {
			switch (skillNum)
			{
			case 0:
				if (!isHit) {
					float _wallAngle = getAngle(BOSSMANAGER->getVector()[i]->getRect().left + 30, BOSSMANAGER->getVector()[i]->getRect().top + 30, _player->getX(), _player->getY());
					int damage = RANDOM->range(8, 12);
					_player->damage(damage, _wallAngle, 10);
					isHit = true;
				}
				if (isHit) {
					hitTimer++;
					if (hitTimer > 20) {
						hitTimer = 0;
						isHit = false;
					}
				}
				break;
			case 2:
				if (!isHit) {
					float _thunderAngle = getAngle(BOSSMANAGER->getVector()[i]->getRect().left + 350, BOSSMANAGER->getVector()[i]->getRect().top + 350, _player->getX(), _player->getY());
					int damage = RANDOM->range(3, 8);
					_player->damage(damage, _thunderAngle, 10);
					isHit = true;
				}
				if (isHit) {
					hitTimer++;
					if (hitTimer > 20) {
						hitTimer = 0;
						isHit = false;
					}
				}
				break;
			case 3:
				if (!isHit) {
					float _windAngle = getAngle(BOSSMANAGER->getVector()[i]->getRect().left + 220, BOSSMANAGER->getVector()[i]->getRect().top + 170, _player->getX(), _player->getY());
					int damage = RANDOM->range(12, 24);
					_player->damage(damage, _windAngle, 20);
					isHit = true;
				}
				if (isHit) {
					hitTimer++;
					if (hitTimer > 20) {
						hitTimer = 0;
						isHit = false;
					}
				}
				break;
			}
		}
	}
	if (skillNum == 1) {
		for (int i = 0; i < blazeBlock.size(); i++) {
			if (IntersectRect(&temp, &_player->getRect(), &blazeBlock[i]->rc)) {
				if (!blazeBlock[i]->isHit) {
					float _blazeAngle = getAngle(blazeBlock[i]->center.x, blazeBlock[i]->center.y, _player->getX(), _player->getY());
					int damage = RANDOM->range(11, 18);
					_player->damage(damage, _blazeAngle, 20);
					blazeBlock[i]->isHit = true;
				}
			}
		}
	}
	if (skillNum == 4) {
		for (int i = 0; i < iceBlock.size(); i++) {
			if (IntersectRect(&temp, &_player->getRect(), &iceBlock[i]->rc)) {
				if (!iceBlock[i]->isHit) {
					float _windAngle = getAngle(iceBlock[i]->center.x, iceBlock[i]->center.y, _player->getX(), _player->getY());
					int damage = RANDOM->range(25, 32);
					_player->damage(damage, _windAngle, 10);
					iceBlock[i]->isHit = true;
				}
			}
		}
	}
}

void finalBoss::finalBossHpInfo(HDC hdc, int destX, int destY)
{
	image* img = IMAGEMANAGER->findImage("bossframebar");
	img->render(hdc, destX, destY);

	img = IMAGEMANAGER->findImage("bosshpbar");

	int hpBar = (float)img->getWidth() * ((float)boss.bossHp / 2000);
	img->render(hdc, destX + 48, destY + 20, 0, 0, hpBar, img->getHeight());
}

