#include "stdafx.h"
#include "bullet.h"
//=============================================================
//	## attack ## (공용총알)
//=============================================================

HRESULT attack::init(const char* imageName, int bulletMax, float range, float ghoulLargeRange, bool isFrameImg)
{
	//총알 이미지 초기화
	_imageName = imageName;
	//총알갯수 및 사거리 초기화
	_bulletMax = bulletMax;
	_range = range;
	_ghoulLargeRange = ghoulLargeRange;
	//총알 이미지가 프레임 이미지냐?
	_isFrameImg = isFrameImg;

	return S_OK;
}

void attack::release()
{
}

void attack::update()
{
	this->move();
}

void attack::render()
{
	int size = _vAttack.size();
	for (int i = 0; i < size; i++)
	{
		//CAMERAMANAGER->Rectangle(getMemDC(), _vAttack[i].rc);
		if (_vAttack[i].bulletImage)
			CAMERAMANAGER->FrameRender(getMemDC(), _vAttack[i].bulletImage,
				_vAttack[i].x - (_vAttack[i].bulletImage->getFrameWidth() >> 1),
				_vAttack[i].y - (_vAttack[i].bulletImage->getFrameHeight() >> 1),
				_vAttack[i].FrameX, _vAttack[i].FrameY);
	}
}

void attack::fire(float x, float y, float angle, float speed, int damage, MONSTERKIND kind)
{
	//총알 벡터에 담는것을 제한한다
	//if (_bulletMax < _vAttack.size() + 1) return;

	//총알 구조체 선언
	tagBullet attack;
	//총알 구조체 초기화
	//제로메모리, 멤셋
	//구조체 변수들의 값을 한번에 0으로 초기화 시켜준다
	//ZeroMemory(&attack, sizeof(tagBullet));
	//attack.bulletImage = IMAGEMANAGER->findImage(_imageName);
	if (kind == MONSTERKIND::SUMMONER)
	{
		attack.bulletImage = IMAGEMANAGER->findImage(_imageName);
	}
	else
	{
		attack.bulletImage = NULL;
	}
	attack.kind = kind;
	attack.speed = speed;
	attack.angle = angle;
	attack.x = attack.fireX = x;
	attack.y = attack.fireY = y;
	attack.radius = 36;
	attack.atkPower = damage;
	attack.count = 0;
	attack.FrameX = 0;
	attack.FrameY = 0;


	//벡터에 담기
	_vAttack.push_back(attack);
}

void attack::move()
{
	int size = _vAttack.size();
	for (int i = 0; i < size; i++)
	{
		_vAttack[i].FrameX = 1;
		_vAttack[i].x += cosf(_vAttack[i].angle) * _vAttack[i].speed;
		_vAttack[i].y += -sinf(_vAttack[i].angle) * _vAttack[i].speed;

		switch (_vAttack[i].kind)
		{
		case MONSTERKIND::GOLEM:
			_vAttack[i].rc = RectMake(_vAttack[i].x, _vAttack[i].y, 130, 200);
			_vAttack[i].count++;
			if (_vAttack[i].count >= 10)
			{
				_vAttack.erase(_vAttack.begin() + i);
				size = _vAttack.size();
			}
			break;
		case MONSTERKIND::KNIGHT:
			_vAttack[i].rc = RectMake(_vAttack[i].x, _vAttack[i].y, 110, 150);
			_vAttack[i].count++;
			if (_vAttack[i].count >= 5)
			{
				_vAttack.erase(_vAttack.begin() + i);
				size = _vAttack.size();
			}
			break;
		case MONSTERKIND::SUMMONER:
		{
			_vAttack[i].rc = RectMakeCenter(_vAttack[i].x, _vAttack[i].y, 30, 30);
			float distance = getDistance(_vAttack[i].fireX, _vAttack[i].fireY,
				_vAttack[i].x, _vAttack[i].y);
			if (_range < distance)//총알이 사거리 보다 커졌을때
			{
				_vAttack.erase(_vAttack.begin() + i);
				size = _vAttack.size();
			}
		}
		break;
		case MONSTERKIND::GHOUL:
			_vAttack[i].rc = RectMake(_vAttack[i].x, _vAttack[i].y, 90, 130);
			_vAttack[i].count++;
			if (_vAttack[i].count >= 5)
			{
				_vAttack.erase(_vAttack.begin() + i);
				size = _vAttack.size();
			}
			break;
		case MONSTERKIND::GHOULLARGE:
			_vAttack[i].rc = RectMakeCenter(_vAttack[i].x, _vAttack[i].y, 120, 140);
			_vAttack[i].count++;
			float distance = getDistance(_vAttack[i].fireX, _vAttack[i].fireY,
				_vAttack[i].x, _vAttack[i].y);
			if (_ghoulLargeRange < distance)//총알이 사거리 보다 커졌을때
			{
				_vAttack.erase(_vAttack.begin() + i);
				size = _vAttack.size();
			}
			break;
		}

	}
}

//총알삭제
void attack::removeAttack(int index)
{
	_vAttack.erase(_vAttack.begin() + index);
}
//수정 중(player flame strike)
//=============================================================
//	## bomb ## (폭탄처럼 한발씩 발사하고 생성하고 자동삭제)
//=============================================================
HRESULT bomb::init(int maxBullet)
{
	IMAGEMANAGER->addFrameImage("blaze", "resource/player/blaze.bmp", 288, 96, 3, 1, true, RGB(255, 0, 255));
	bombCount = bTime = 0;

	//총알 사거리 및 총알 갯수 초기화
	_bulletMax = maxBullet;

	count = index = 0;

	bossScene = false;
	isCoolTime = false;
	info.coolTime = 40;
	currentCoolTime = 0;

	info.keyName = "skill_blaze";
	info.explanation = "블레이즈[기초]";
	return S_OK;
}

void bomb::release()
{
}

void bomb::update()
{
	bombActive();

	count++;

	if (count % 2 == 0)
	{
		index++;
		if (index > 3)index = 0;
	}
	this->move();

	if (isCoolTime)
	{
		currentCoolTime++;
		if (currentCoolTime == info.coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}
}

void bomb::render()
{
	int size = _vBullet.size();
	for (int i = 0; i < size; i++)
	{
		CAMERAMANAGER->FrameRender(getMemDC(), _vBullet[i].bulletImage,
			_vBullet[i].x - (_vBullet[i].bulletImage->getFrameWidth() / 2),
			_vBullet[i].y - (_vBullet[i].bulletImage->getFrameHeight() / 2), index, 0);

		if (INPUT->GetToggleKey('L'))CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 20, 20));
	}
}

float bomb::getRange(float angle, float x, float y)
{
	float tmpIndex = 10;

	for (int i = 0; i < 10; i++)
	{
		//32마다 비교
		float posX = x + cosf(angle) * (32 * i);
		float posY = y - sinf(angle) * (32 * i);
		RECT rTmp = RectMakeCenter(posX, posY, 4, 4);

		int size = PLAYERDATA->getWall().size();
		for (int j = 0; j < size; j++)
		{
			int num = PLAYERDATA->getWall()[j];

			if (colCheck(rTmp, PLAYERDATA->_getTile()[num].rc) == false) continue;

			return i * 32;
		}
	}

	return 320.f;
}

void bomb::fire(float x, float y, float speed, float angle)
{
	if (_bulletMax < _vBullet.size() + 1) return;

	//총알 구조체 선언
	tagBomb bullet;
	bullet.bulletImage = IMAGEMANAGER->findImage("blaze");
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.speed = speed;
	bullet.angle = angle;
	bullet.collision = false;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y, 4, 4);
	bullet.atkPower = 12;

	bullet.range = getRange(angle, x, y);	// 벽에 충돌하지않는 선에서 최대 거리 계산

	//tmpBomb에 저장
	tmpBomb = bullet;
	bombCount = 3;

	isCoolTime = true;
	UI->addCoolTime(info.keyName);
	//sound

}

void bomb::bombActive()
{
	if (bombCount == 0) return;

	bTime++;

	if (bTime % 10 == 0)
	{
		_vBullet.push_back(tmpBomb);

		//생성 완료
		bombCount--;

		//bTime 초기화
		bTime = 0;

		SOUNDMANAGER->play("blazeFire", false);
	}
}

void bomb::move() // blaze tile충돌은 gameScene에서만 되도록 처리하기
{
	int size = _vBullet.size();
	for (int i = 0; i < size;)
	{
		_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, 4, 4);

		float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY, _vBullet[i].x, _vBullet[i].y);

		if (_vBullet[i].range < distance || _vBullet[i].collision)
		{
			removeBomb(i);
			size = _vBullet.size();
		}
		else i++;
	}
}
//폭탄삭제
void bomb::removeBomb(int index)
{
	PARTICLE->explosionGenerate("explosionParticle", _vBullet[index].x + 20, _vBullet[index].y + 20, 5, 30, 2.f, 3, true);
	SOUNDMANAGER->play("blazeExp", false, -0.15f);

	_vBullet.erase(_vBullet.begin() + index);
}

//수정 중(player meteor)
//=============================================================
//	## meteor ## (signature)
//=============================================================
HRESULT meteor::init()
{
	IMAGEMANAGER->addFrameImage("circle", "resource/player/castingCircle1.bmp", 3072, 128, 24, 1, true, RGB(255, 0, 0));
	IMAGEMANAGER->addFrameImage("meteor", "resource/player/meteor.bmp", 1200, 250, 6, 1);
	currentCoolTime = 0;

	isCoolTime = false;

	count = index = 0;


	//upgrade용
	upgrade = false;
	ranCount = 5;
	save = { 0,0 };
	meteorCount = 0;

	info.keyName = "skill_meteor";
	info.explanation = "메테오[시그니쳐]";
	info.coolTime = 300;
	return S_OK;
}

void meteor::release()
{
}

void meteor::update()
{

	count++;

	coolTimeReduction();
	move();

	if (isCoolTime)
	{
		currentCoolTime++;

		if (currentCoolTime == info.coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}

	//데미지 넣을 용도
	for (int i = 0; i < vDamage.size();)
	{
		vDamage[i].lifeTime--;

		if (vDamage[i].lifeTime == 0)
		{
			vDamage.erase(vDamage.begin());
		}
		else
		{
			i++;
		}
	}

	//upgrade meteor
	if (meteorCount > 0)
	{
		//count = 7 % 5 == 2
		if (count % ranCount == 0)
		{
			ranCount = RANDOM->range(10, 20);

			float ranX = RANDOM->range(-150.f, 150.f);
			float ranY = RANDOM->range(-150.f, 150.f);
			tagMeteor meteor;
			meteor.endY = save.y + ranY;
			meteor.img = IMAGEMANAGER->findImage("meteor");

			float cul = 110 * (PI / 180);
			meteor.x = (save.x + ranX) + cosf(cul) * 600.f;
			meteor.y = (save.y + ranY) - sinf(cul) * 600.f;
			meteor.rc = RectMakeCenter(meteor.x, meteor.y, 100, 100);
			meteor.angle = 290 * (PI / 180);
			meteor.speed = 15.f;

			vMeteor.push_back(meteor);
			makeCircle(save.x + ranX, save.y + ranY);

			isCoolTime = true;
			UI->addCoolTime(info.keyName);
			meteorCount--;
		}

	}

}

void meteor::render()
{
	int size = vCircle.size();
	for (int i = 0; i < size; i++)
	{
		vCircle[i].count++;
		if (vCircle[i].count % 3 == 0)
		{
			if (vCircle[i].index < 23)
				vCircle[i].index++;
		}

		//CAMERAMANAGER->Ellipse(getMemDC(), vCircle[i].rc);
		CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("circle"),
			vCircle[i].x - (IMAGEMANAGER->findImage("circle")->getFrameWidth() >> 1),
			vCircle[i].y - (IMAGEMANAGER->findImage("circle")->getFrameHeight() >> 1),
			vCircle[i].index, 0);
	}

	size = vMeteor.size();
	for (int i = 0; i < size; i++)
	{
		if (count % 5 == 0)
		{
			index++;
			if (index > 5) index = 0;
		}

		CAMERAMANAGER->FrameRender(getMemDC(), vMeteor[i].img,
			vMeteor[i].x - (vMeteor[i].img->getFrameWidth() >> 1) - 10,
			vMeteor[i].y - (vMeteor[i].img->getFrameHeight() >> 1) - 50,
			index, 0);
	}
	if (INPUT->GetToggleKey('L'))
	{
		for (int i = 0; i < vDamage.size(); i++)
		{
			CAMERAMANAGER->Ellipse(getMemDC(), vDamage[i].rc);
		}
	}
}

void meteor::makeCircle(float x, float y)
{
	tagCircle circle;

	circle.angle = 0;
	circle.x = x;
	circle.y = y;
	circle.count = 0;
	circle.index = 0;
	circle.rc = RectMakeCenter(circle.x, circle.y, 100, 100);
	circle.lifeTime = 0;

	vCircle.push_back(circle);
}

void meteor::creatMeteor(float x, float y, float angle)
{
	char meteo_sound[20];
	sprintf(meteo_sound, "meteoFire%d", RANDOM->range(4));
	if (upgrade == false)
	{
		tagMeteor meteor;
		meteor.endY = y;
		meteor.img = IMAGEMANAGER->findImage("meteor");

		float cul = 110 * (PI / 180);
		meteor.x = x + cosf(cul) * 600.f;
		meteor.y = y - sinf(cul) * 600.f;
		meteor.rc = RectMakeCenter(meteor.x, meteor.y, 100, 100);
		meteor.angle = 290 * (PI / 180);
		meteor.speed = 15.f;

		vMeteor.push_back(meteor);
		makeCircle(x, y);
		isCoolTime = true;
		UI->addCoolTime(info.keyName);


		SOUNDMANAGER->play(meteo_sound, false);
	}

	else
	{
		save = { (long)x,(long)y };
		meteorCount = 5;
		ranCount = 5;
		SOUNDMANAGER->play(meteo_sound, false);
	}

}



void meteor::move()
{
	int size = vMeteor.size();
	for (int i = 0; i < size;)
	{
		if (count % 2 == 0)PARTICLE->smokeParticlePlay(vMeteor[i].x, vMeteor[i].y);

		//이동
		vMeteor[i].x += cosf(vMeteor[i].angle) * vMeteor[i].speed;
		vMeteor[i].y -= sinf(vMeteor[i].angle) * vMeteor[i].speed;
		vMeteor[i].rc = RectMakeCenter(vMeteor[i].x, vMeteor[i].y, 100, 100);

		//삭제
		if (vMeteor[i].y > vMeteor[i].endY)
		{
			PARTICLE->explosionParticle2Play(vMeteor[i].x, vMeteor[i].y);

			//데미지 넣을 용도
			tagMeteor damage;

			damage.x = vCircle[i].x;
			damage.y = vCircle[i].y;
			damage.angle = vMeteor[i].angle; //메테오 방향으로 넉백시킬 때 사용
			damage.lifeTime = 30;
			damage.rc = RectMakeCenter(damage.x, damage.y, 100, 100);
			damage.atkPower = 20;

			vDamage.push_back(damage);

			CAMERAMANAGER->Shake(20, 20, 4);

			index = 0;
			vMeteor.erase(vMeteor.begin() + i);
			vCircle.erase(vCircle.begin() + i);
			size = vMeteor.size();
			char meteo_sound[20];
			sprintf(meteo_sound, "meteoExp%d", RANDOM->range(4));
			SOUNDMANAGER->play(meteo_sound, false);
		}
		else i++;
	}
}

void meteor::coolTimeReduction()
{

	if (PLAYERDATA->getStat().CoolTimeReduction != 0)
	{
		info.coolTime = 300 - (PLAYERDATA->getStat().CoolTimeReduction * 60);
		UI->fixCoolTime(info.keyName, info.coolTime);

	}
	else
	{
		info.coolTime = 300;
		UI->fixCoolTime(info.keyName, info.coolTime);
	}

}


//DASH
HRESULT dashFire::init()
{
	isCoolTime = false;
	currentCoolTime = 0;


	info.keyName = "skill_searingDash";
	info.explanation = "대쉬[파이어]";
	info.coolTime = 240;

	return S_OK;
}

void dashFire::release()
{
}

void dashFire::update()
{
	coolTimeReduction();

	if (isCoolTime)
	{
		currentCoolTime++;
		if (currentCoolTime == info.coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}
}

//사용X
void dashFire::render()
{
	//for (int i = 0; i < _vDash.size();)
	//{
	//	_vDash[i].lifeTime++;

	//	//CAMERAMANAGER->Ellipse(getMemDC(), _vDash[i].rc);
	//	image* img = IMAGEMANAGER->findImage("flame");

	//	CAMERAMANAGER->FrameRender(getMemDC(), img,
	//		_vDash[i].x - img->getFrameWidth() / 2,
	//		_vDash[i].y - img->getFrameHeight() / 2, _vDash[i].frameX, 0);

	//	if (_vDash[i].lifeTime == 180)
	//	{
	//		_vDash.erase(_vDash.begin() + i);
	//	}
	//	else
	//	{
	//		if (_vDash[i].lifeTime % 5 == 0) _vDash[i].frameX++;
	//		if (_vDash[i].frameX == img->getMaxFrameX()) _vDash[i].frameX = 0;

	//		i++;
	//	}
	//}
}

void dashFire::singleRender(int index)
{
	vDash[index].lifeTime++;

	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("flame"),
		vDash[index].x - (IMAGEMANAGER->findImage("flame")->getFrameWidth() >> 1),
		vDash[index].y - (IMAGEMANAGER->findImage("flame")->getFrameHeight() >> 1) - 20,
		vDash[index].frameX, 0);

	if (INPUT->GetToggleKey('L'))	CAMERAMANAGER->Ellipse(getMemDC(), vDash[index].rc);
	if (vDash[index].lifeTime == 180)
	{
		vDash.erase(vDash.begin() + index);
	}
	else
	{
		if (vDash[index].lifeTime % 5 == 0) vDash[index].frameX++;
		if (vDash[index].frameX == IMAGEMANAGER->findImage("flame")->getMaxFrameX()) vDash[index].frameX = 0;
	}
}

void dashFire::fire(float x, float y)
{
	tagArcana dash;
	dash.rc = RectMakeCenter(x, y, 50, 50);
	dash.x = x;
	dash.y = y;
	dash.frameX = 0;
	dash.lifeTime = 0;
	dash.atkPower = 7;
	vDash.push_back(dash);
}


void dashFire::coolTimeReduction()
{
	if (PLAYERDATA->getStat().CoolTimeReduction != 0)
	{
		info.coolTime = 240 - (PLAYERDATA->getStat().CoolTimeReduction * 60);
		UI->fixCoolTime(info.keyName, info.coolTime);
	}
	else
	{
		info.coolTime = 240;
		UI->fixCoolTime(info.keyName, info.coolTime);
	}

}


//추가 스킬 RAGING INFERNO

HRESULT RagingInferno::init()
{

	inferno.img = IMAGEMANAGER->addFrameImage("inferno", "resource/player/inferno.bmp", 240, 80, 3, 1);

	time = 0;
	gaugeTime = 0;
	distance = 0;
	currentCoolTime = 0;

	isFire = gauging = isActive = false;
	isCoolTime = false;


	info.keyName = "skill_inferno";
	info.explanation = "인페르노[스탠다드]";
	info.coolTime = 300;

	index = count = 0;

	return S_OK;
}

void RagingInferno::release()
{
}

void RagingInferno::update()
{
	count++;
	gaugeTime++;
	coolTimeReduction();

	PLAYERDATA->setGaugeTime(gaugeTime);
	if (count % 3 == 0)
	{
		index++;

		if (index > 2) index = 0;
	}

	distance = getDistance(inferno.x, inferno.y, inferno.fireX, inferno.fireY);

	move();
	if (isActive && gaugeTime < 50)
	{
		gaugeTime = 50;
		inferno.lifeTime--;
		if (PLAYERDATA->getCurrentStage() == STAGEKIND::STARTSTAGE)
			SOUNDMANAGER->play("RagingInfernoExp", false);
		else
			SOUNDMANAGER->play("RagingInfernoExpBoss", false);
		
	}

	if (isCoolTime)
	{
		currentCoolTime++;

		if (currentCoolTime == info.coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}

}

void RagingInferno::render()
{
	if (isFire)
	{
		CAMERAMANAGER->FrameRender(getMemDC(), inferno.img,
			inferno.x - (inferno.img->getFrameWidth() / 2),
			inferno.y - (inferno.img->getFrameHeight() / 2), index, 0);

		if (PLAYERDATA->getGaugeTime() >= 50)
		{
			int size = vTail.size();
			for (int i = 0; i < size;)
			{
				//이동
				vTail[i].x += cosf(vTail[i].angle) * vTail[i].speed;
				vTail[i].y -= sinf(vTail[i].angle) * vTail[i].speed;

				if (10.f > vTail[i].minAngle)
				{
					vTail[i].minAngle += 0.0012f;
				}
				vTail[i].angle -= sinf(vTail[i].minAngle);

				//렌더
				//CAMERAMANAGER->Ellipse(getMemDC(), RectMakeCenter(vTail[i].x, vTail[i].y, 20, 20));
				PARTICLE->generate("frameParticle", vTail[i].x, vTail[i].y, vTail[i].angle, 4, 1.f, true);

				//삭제
				if (vTail[i].currentTime == vTail[i].lifeTime)
				{
					vTail.erase(vTail.begin() + i);
					size = vTail.size();
				}
				else
				{
					vTail[i].currentTime++;

					i++;
				}
			}
		}
		//CAMERAMANAGER->Ellipse(getMemDC(), inferno.rc);
	}
	else
	{
		if (vTail.empty() == false)vTail.clear();
	}
}

void RagingInferno::fire(float x, float y, float angle)
{
	inferno.angle = angle;
	inferno.speed = -30.f;
	inferno.x = inferno.fireX = x + cosf(angle) * inferno.speed;
	inferno.y = inferno.fireY = y - sinf(angle) * inferno.speed;
	inferno.rc = RectMakeCenter(inferno.x, inferno.y, 30, 30);
	inferno.lifeTime = 100;
	inferno.atkPower = 15;
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 3, 3.f, 0.8f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 5, 5.f, 0.7f, 10);
	PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 60, 7, 7.f, 0.6f, 10);
	SOUNDMANAGER->play("RagingInfernoFire", false);
	gaugeTime = 0;
	isFire = true;
	gauging = true;
	PLAYERDATA->setGauging(gauging);
	isCoolTime = true;
	UI->addCoolTime(info.keyName);
}

void RagingInferno::move()
{
	if (isFire)
	{
		if (gaugeTime > 30 && gaugeTime < 50)
		{

			inferno.x = inferno.x + cosf(inferno.angle) * 20.0f;
			inferno.y = inferno.y - sinf(inferno.angle) * 20.0f;
			inferno.rc = RectMakeCenter(inferno.x, inferno.y, 70, 70);
			gauging = false;
			PLAYERDATA->setGauging(gauging);
			if (gaugeTime % 3 == 0) PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 1, 6, 6, 20.f, 0.4f, 10);
		}
		else isActive = false;
		if (gaugeTime == 50)
		{
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 3, 3.f, 0.8f, 10);
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 5, 5.f, 0.7f, 10);
			PARTICLE->pointGenerate("frameParticle", inferno.x, inferno.y, 2, 100, 7, 7.f, 0.6f, 10);
			if (PLAYERDATA->getCurrentStage() == STAGEKIND::STARTSTAGE)
				SOUNDMANAGER->play("RagingInfernoExp", false);
			else
				SOUNDMANAGER->play("RagingInfernoExpBoss", false);
		}

		if (gaugeTime >= 50)
		{
			isActive = true;
			inferno.rc = RectMakeCenter(inferno.x, inferno.y, 150, 150);

			inferno.lifeTime--;

			if (time % 35 == 0)
			{
				tagTail tail;
				tail.currentTime = 0;
				tail.minAngle = 0.05f;
				tail.lifeTime = 150;
				tail.speed = 8.f;
				float angle = 0;

				tail.x = inferno.x - 37;
				tail.y = inferno.y - 120;
				tail.angle = angle;

				vTail.push_back(tail);

				angle = 120 * (PI / 180);
				tail.x = inferno.x - 82;
				tail.y = inferno.y + 85;
				tail.angle = angle;

				vTail.push_back(tail);

				angle = 240 * (PI / 180);
				tail.x = inferno.x + 113;
				tail.y = inferno.y + 20;
				tail.angle = angle;

				vTail.push_back(tail);
			}
			time++;


		}

		if (inferno.lifeTime == 0)
		{
			PARTICLE->explosionParticlePlay(inferno.x, inferno.y);
			CAMERAMANAGER->Shake(20, 20, 5);
			inferno.x = inferno.fireX;
			inferno.y = inferno.fireY;
			isFire = false;
			isActive = false;
		}
	}
}

void RagingInferno::coolTimeReduction()
{
	if (PLAYERDATA->getStat().CoolTimeReduction != 0)
	{
		info.coolTime = 240 - (PLAYERDATA->getStat().CoolTimeReduction * 60);
		UI->fixCoolTime(info.keyName, info.coolTime);
	}
	else
	{
		info.coolTime = 240;
		UI->fixCoolTime(info.keyName, info.coolTime);
	}
}

bool RagingInferno::CheckCollision(RECT enemy)
{
	RECT temp;
	if (IntersectRect(&temp, &inferno.rc, &enemy))
	{
		return true;
	}
	else return false;

}


//=============================================================
//	## dragonArc ## 
//=============================================================

HRESULT dragonArc::init()
{
	IMAGEMANAGER->addFrameImage("dragon", "resource/player/dragonArc.bmp", 1800 * 3, 100 * 3, 36, 2);
	count = index = 0;


	//memset(&dragonHead, 0, sizeof(dragonHead));

	currentCoolTime = 0;
	isCoolTime = false;
	upgrade = false;


	info.keyName = "skill_dragonArc";
	info.explanation = "불꽃 용[시그니쳐]";
	info.coolTime = 300;
	pattern = 0;
	return S_OK;
}

void dragonArc::release()
{

}

void dragonArc::update()
{
	// ui에 dragonArc 아이콘 추가하기 + 쿨타임 세팅하기

	count++;

	//coolTime
	if (isCoolTime)
	{
		currentCoolTime++;
		if (currentCoolTime == info.coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}


	//	coolTimeReduction();
	move();

	phoenixMove();

}

void dragonArc::render()
{
	if (!upgrade)
	{
		int size = vDragon.size();
		for (int i = 0; i < size; i++)
		{

			if (count % 2 == 0)
			{
				PARTICLE->explosionGenerate("explosionParticle2", vDragon[i].x, vDragon[i].y, 3, 0.75f, 0.f, 3, true);
				//PARTICLE->explosionGenerate("explosionParticle2", vDragon[i].x, vDragon[i].y, 5, 1.25f, 0.f, 1, true);
			}

			CAMERAMANAGER->FrameRender(getMemDC(), vDragon[i].img,
				vDragon[i].x - (vDragon[i].img->getFrameWidth() >> 1),
				vDragon[i].y - (vDragon[i].img->getFrameHeight() >> 1),
				vDragon[i].index, vDragon[i].frameY);

			//CAMERAMANAGER->Ellipse(getMemDC(), vDragon[i].rc);
		}
	}

	if (dragonHead.isFire)
	{
		image* img = IMAGEMANAGER->findImage("dragon");
		//for (int i = 0; i < vColRound.size(); i++)
			//CAMERAMANAGER->Ellipse(getMemDC(), vColRound[i].rc);
		if (count % 2 == 0) PARTICLE->explosionGenerate("frameParticle", dragonHead.x, dragonHead.y, 1, 0, 0.f, 2, true);
		//CAMERAMANAGER->FrameRender(getMemDC(), img,
		//	dragonHead.x - img->getFrameWidth() / 2,
		//	dragonHead.y - img->getFrameHeight() / 2, dragonHead.index, dragonHead.frameY);

		int size = vWings.size();
		for (int i = 0; i < size; i++)
		{
			//CAMERAMANAGER->Ellipse(getMemDC(), vWings[i].rc);
			if (count % 2 == 0)
			{
				PARTICLE->explosionGenerate("explosionParticle", vWings[i].x, vWings[i].y, 3, 7, 0.f, 2, true);
			}
		}
	}

}

void dragonArc::fire(float x, float y, float angle)
{
	switch (pattern)
	{
	case 0:
		SOUNDMANAGER->play("fireDragon", false, -0.2f);

		tagDragon dragon;

		//float ptAngle = getAngle(x, y, _ptMouse.x, _ptMouse.y);
		dragon.img = IMAGEMANAGER->findImage("dragon");
		dragon.x = x;
		dragon.y = y;
		dragon.currentTime = 0;
		dragon.lifeTime = 70;
		dragon.speed = 15.f;
		dragon.rc = RectMakeCenter(dragon.x, dragon.y, 50, 50);

		dragon.angle = .7f + angle;
		dragon.saveAngle = -.7f + angle;
		dragon.frameY = 0;
		dragon.index = angle * 18 / PI;//0-35
		dragon.atkPower = 15;


		vDragon.push_back(dragon);

		dragon.angle = -.7f + angle;
		dragon.saveAngle = .7f + angle;

		dragon.frameY = 1;
		dragon.index = 36 - (angle * 18 / PI);//0-35

		vDragon.push_back(dragon);
		UI->addCoolTime(info.keyName);
		break;
	case 1:
		if (vDragon.size() < 2)
		{
			tagDragon dragon;

			dragon.img = IMAGEMANAGER->findImage("dragon");
			dragon.x = x;
			dragon.y = y;
			dragon.currentTime = 0;
			dragon.lifeTime = 88;
			dragon.speed = 15.f;
			dragon.rc = RectMakeCenter(dragon.x, dragon.y, 60, 60);

			dragon.angle = .7f + angle;
			dragon.saveAngle = -.7f + angle;
			dragon.frameY = 0;
			dragon.index = angle * 18 / PI;//0-35
			dragon.atkPower = 15;

			vDragon.push_back(dragon);

			dragon.index = 0;
			dragon.angle = -.7f + angle;
			dragon.saveAngle = .7f + angle;

			dragon.frameY = 1;
			dragon.index = 36 - (angle * 18 / PI);//0-35

			vDragon.push_back(dragon);
			UI->addCoolTime(info.keyName);

		}
		break;
	}

	isCoolTime = true;
}

void dragonArc::move()
{
	int size;
	switch (pattern)
	{
	case 0:
		size = vDragon.size();
		for (int i = 0; i < size;)
		{
			if (vDragon[i].currentTime < 21)
			{
				vDragon[i].x += cosf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].y -= sinf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].rc = RectMakeCenter(vDragon[i].x, vDragon[i].y, 50, 50);

				if (i % 2 == 0) vDragon[i].angle -= sinf(0.07f);
				else vDragon[i].angle += sinf(0.07f);

				if (vDragon[i].currentTime == 20)
				{
					if (i % 2 == 0)
					{
						vDragon[i].angle = vDragon[i].saveAngle - 0.07f;
					}
					else vDragon[i].angle = vDragon[i].saveAngle + 0.07f;
				}
			}

			else if (vDragon[i].currentTime < 44)
			{
				vDragon[i].x += cosf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].y -= sinf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].rc = RectMakeCenter(vDragon[i].x, vDragon[i].y, 50, 50);

				if (i % 2 == 0) vDragon[i].angle += sinf(0.07f);
				else vDragon[i].angle -= sinf(0.07f);
			}

			else
			{
				vDragon[i].x += cosf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].y -= sinf(vDragon[i].angle) * vDragon[i].speed;

				if (i % 2 == 0) vDragon[i].angle -= sinf(0.07f);
				else vDragon[i].angle += sinf(0.07f);
			}

			if (vDragon[i].currentTime == vDragon[i].lifeTime)
			{
				vDragon.erase(vDragon.begin() + i);
				size = vDragon.size();
			}
			else
			{
				vDragon[i].currentTime++;

				i++;
			}
		}

		break;
	case 1:
		size = vDragon.size();
		for (int i = 0; i < size;)
		{
			if (vDragon[i].currentTime < 35 || vDragon[i].currentTime > 69)
			{
				vDragon[i].x += cosf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].y -= sinf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].rc = RectMakeCenter(vDragon[i].x, vDragon[i].y, 60, 60);
				if (i % 2 == 0)
				{
					vDragon[i].angle -= sinf(0.05f);
				}
				else
				{
					vDragon[i].angle += sinf(0.05f);
				}

				if (vDragon[i].currentTime == 34)
				{
					vDragon[i].angle = vDragon[i].saveAngle;
					vDragon[i].speed -= 2.6f;

					if (i % 2 == 0)
					{
						vDragon[i].frameY = 1;
						vDragon[i].index = abs(36 - vDragon[i].index);
					}

					else
					{
						vDragon[i].frameY = 0;
						vDragon[i].index = abs(36 - vDragon[i].index);
					}
				}
			}
			else
			{
				vDragon[i].x += cosf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].y -= sinf(vDragon[i].angle) * vDragon[i].speed;
				vDragon[i].rc = RectMakeCenter(vDragon[i].x, vDragon[i].y, 20, 20);
				if (i % 2 == 0)
				{
					vDragon[i].angle += sinf(0.05f);
				}

				else
				{
					vDragon[i].angle -= sinf(0.05f);
				}

				if (vDragon[i].currentTime == 69)
				{
					vDragon[i].angle = vDragon[i].saveAngle;

					if (i % 2 == 0)
						vDragon[i].angle = vDragon[i].saveAngle + PI / 2;

					else vDragon[i].angle = vDragon[i].saveAngle - PI / 2;

					vDragon[i].speed += 1.6f;
				}
			}

			if (vDragon[i].currentTime == vDragon[i].lifeTime)
			{
				vDragon.erase(vDragon.begin() + i);
				size = vDragon.size();
			}
			else
			{
				vDragon[i].currentTime++;
				i++;
			}
		}
		break;
	}

}

void dragonArc::phoenixFire(float x, float y, float angle)
{
	//fire
	if (dragonHead.isFire == false)
	{
		SOUNDMANAGER->play("fireDragonUpgrade", false);
		dragonHead.img = IMAGEMANAGER->findImage("dragon");
		dragonHead.isFire = true;
		dragonHead.currentTime = 0;
		dragonHead.angle = angle;//getAngle(player.x, player.y, _ptMouse.x, _ptMouse.y);
		dragonHead.x = x + cosf(dragonHead.angle) * 50.f;
		dragonHead.y = y - sinf(dragonHead.angle) * 50.f;
		dragonHead.lifeTime = 200;
		dragonHead.speed = 7.f;
		dragonHead.persent = 0.8f;
		dragonHead.rc = RectMakeCenter(dragonHead.x, dragonHead.y, 40, 40);
		dragonHead.frameY = 0;
		dragonHead.index = angle * (18 / PI);

		tagDragonCol col;
		col.angle = angle;
		col.x = x;
		col.y = y;
		col.rc = RectMakeCenter(col.x, col.y, 200, 200);
		vColRound.push_back(col);


		//bullet x 2
		tagDragon wings;
		wings.currentTime = 0;
		wings.angle = dragonHead.angle;
		wings.x = x;
		wings.y = y;
		wings.lifeTime = 200;
		wings.speed = 7.f;
		wings.rc = RectMakeCenter(wings.x, wings.y, 40, 40);
		wings.atkPower = 25;

		vWings.push_back(wings);
		vWings.push_back(wings);

		//bullet x 2
		vWings.push_back(wings);
		vWings.push_back(wings);

		//bullet x 2
		vWings.push_back(wings);
		vWings.push_back(wings);

		isCoolTime = true;
		UI->addCoolTime(info.keyName);
	}

}
void dragonArc::phoenixMove()
{
	//move
	if (dragonHead.isFire)
	{
		//pos
		dragonHead.x += cosf(dragonHead.angle) * dragonHead.speed * dragonHead.persent;
		dragonHead.y -= sinf(dragonHead.angle) * dragonHead.speed * dragonHead.persent;
		dragonHead.rc = RectMakeCenter(dragonHead.x, dragonHead.y, 40, 40);

		for (int i = 0; i < vColRound.size(); i++)
		{
			vColRound[i].x += cosf(vColRound[i].angle) * dragonHead.speed * dragonHead.persent;
			vColRound[i].y -= sinf(vColRound[i].angle) * dragonHead.speed * dragonHead.persent;
			vColRound[i].rc = RectMakeCenter(vColRound[i].x, vColRound[i].y, 200, 200);

		}
		//del
		if (dragonHead.currentTime == dragonHead.lifeTime)
		{
			dragonHead.isFire = false;
			for (int i = 0; i < vColRound.size(); i++)
				vColRound.erase(vColRound.begin() + i);
		}

		else dragonHead.currentTime++;
	}


	for (int i = 0; i < vWings.size();)
	{
		vWings[i].x += cosf(vWings[i].angle) * vWings[i].speed * dragonHead.persent;
		vWings[i].y -= sinf(vWings[i].angle) * vWings[i].speed * dragonHead.persent;
		vWings[i].rc = RectMakeCenter(vWings[i].x, vWings[i].y, 40, 40);

		if (vWings[i].currentTime < 20)
		{
			switch (i)
			{
			case 0:
				vWings[i].x += cosf(vWings[i].angle + PI / 2) * 3;
				vWings[i].y -= sinf(vWings[i].angle + PI / 2) * 3;
				break;
			case 1:
				vWings[i].x += cosf(vWings[i].angle - PI / 2) * 3;
				vWings[i].y -= sinf(vWings[i].angle - PI / 2) * 3;
				break;
			case 2:
				vWings[i].x += cosf(vWings[i].angle + (290 * (PI / 180))) * 4;
				vWings[i].y -= sinf(vWings[i].angle + (290 * (PI / 180))) * 4;
				break;
			case 3:
				vWings[i].x += cosf(vWings[i].angle - (290 * (PI / 180))) * 4;
				vWings[i].y -= sinf(vWings[i].angle - (290 * (PI / 180))) * 4;
				break;
			case 4:
				vWings[i].x += cosf(vWings[i].angle + PI / 2) * 6;
				vWings[i].y -= sinf(vWings[i].angle + PI / 2) * 6;
				break;
			case 5:
				vWings[i].x += cosf(vWings[i].angle - PI / 2) * 6;
				vWings[i].y -= sinf(vWings[i].angle - PI / 2) * 6;
				break;
			}
		}

		else if (vWings[i].currentTime < 40)
		{
			//2,3 <<-
			if (i == 2 || i == 3)
			{
				vWings[i].x += cosf(vWings[i].angle - PI) * 5;
				vWings[i].y -= sinf(vWings[i].angle - PI) * 5;

				if (i == 2)
				{
					vWings[i].x += cosf(vWings[i].angle + (290 * (PI / 180))) * 2;
					vWings[i].y -= sinf(vWings[i].angle + (290 * (PI / 180))) * 2;
				}

				else
				{
					vWings[i].x += cosf(vWings[i].angle - (290 * (PI / 180))) * 2;
					vWings[i].y -= sinf(vWings[i].angle - (290 * (PI / 180))) * 2;
				}
			}
			//4,5 <-
			else if (i == 4 || i == 5)
			{
				vWings[i].x += cosf(vWings[i].angle - PI) * 5.f;
				vWings[i].y -= sinf(vWings[i].angle - PI) * 5.f;

				if (i == 4)
				{
					vWings[i].x += cosf(vWings[i].angle + PI / 2) * 3;
					vWings[i].y -= sinf(vWings[i].angle + PI / 2) * 3;
				}

				else
				{
					vWings[i].x += cosf(vWings[i].angle - PI / 2) * 3;
					vWings[i].y -= sinf(vWings[i].angle - PI / 2) * 3;
				}
			}
		}

		else if (vWings[i].currentTime > 80 && vWings[i].currentTime < 100)
		{
			//2,3 ->>
			if (i == 2 || i == 3)
			{
				vWings[i].x += cosf(vWings[i].angle) * 4;
				vWings[i].y -= sinf(vWings[i].angle) * 4;

				if (i == 2)
				{
					vWings[i].x += cosf(vWings[i].angle - (290 * (PI / 180))) * 2;
					vWings[i].y -= sinf(vWings[i].angle - (290 * (PI / 180))) * 2;
				}

				else
				{
					vWings[i].x += cosf(vWings[i].angle + (290 * (PI / 180))) * 2;
					vWings[i].y -= sinf(vWings[i].angle + (290 * (PI / 180))) * 2;
				}
			}
			//4,5 ->
			else if (i == 4 || i == 5)
			{
				vWings[i].x += cosf(vWings[i].angle) * 3;
				vWings[i].y -= sinf(vWings[i].angle) * 3;

				if (i == 4)
				{
					vWings[i].x += cosf(vWings[i].angle - PI / 2) * 3;
					vWings[i].y -= sinf(vWings[i].angle - PI / 2) * 3;
				}

				else
				{
					vWings[i].x += cosf(vWings[i].angle + PI / 2) * 3;
					vWings[i].y -= sinf(vWings[i].angle + PI / 2) * 3;
				}
			}
		}

		else if (vWings[i].currentTime > 100 && vWings[i].currentTime < 120)
		{
			//2,3 <<-
			if (i == 2 || i == 3)
			{
				vWings[i].x += cosf(vWings[i].angle - PI) * 5;
				vWings[i].y -= sinf(vWings[i].angle - PI) * 5;

				if (i == 2)
				{
					vWings[i].x += cosf(vWings[i].angle + (290 * (PI / 180))) * 2;
					vWings[i].y -= sinf(vWings[i].angle + (290 * (PI / 180))) * 2;
				}

				else
				{
					vWings[i].x += cosf(vWings[i].angle - (290 * (PI / 180))) * 2;
					vWings[i].y -= sinf(vWings[i].angle - (290 * (PI / 180))) * 2;
				}
			}
			//4,5 <-
			else if (i == 4 || i == 5)
			{
				vWings[i].x += cosf(vWings[i].angle - PI) * 5.f;
				vWings[i].y -= sinf(vWings[i].angle - PI) * 5.f;

				if (i == 4)
				{
					vWings[i].x += cosf(vWings[i].angle + PI / 2) * 3;
					vWings[i].y -= sinf(vWings[i].angle + PI / 2) * 3;
				}

				else
				{
					vWings[i].x += cosf(vWings[i].angle - PI / 2) * 3;
					vWings[i].y -= sinf(vWings[i].angle - PI / 2) * 3;
				}
			}
		}


		if (vWings[i].currentTime < 20)
		{
			if (dragonHead.persent > 0.65f)dragonHead.persent -= 0.005f;
		}
		else if (vWings[i].currentTime < 40)
		{
			if (dragonHead.persent < 1.4f)dragonHead.persent += 0.02f;
		}
		else if (vWings[i].currentTime > 80 && vWings[i].currentTime < 100)
		{
			if (dragonHead.persent > 0.65f)dragonHead.persent -= 0.005f;
		}
		else if (vWings[i].currentTime > 100 && vWings[i].currentTime < 120)
		{
			if (dragonHead.persent < 1.4f)dragonHead.persent += 0.02f;
		}

		if (vWings[i].currentTime == vWings[i].lifeTime) vWings.erase(vWings.begin() + i);
		else
		{
			vWings[i].currentTime++;
			i++;
		}
	}
}

void dragonArc::coolTimeReduction()
{
	if (PLAYERDATA->getStat().CoolTimeReduction != 0)
	{
		info.coolTime = 240 - (PLAYERDATA->getStat().CoolTimeReduction * 60);
		UI->fixCoolTime(info.keyName, info.coolTime);
	}
	else
	{
		info.coolTime = 240;
		UI->fixCoolTime(info.keyName, info.coolTime);
	}
}


// ICE SPEAR


HRESULT iceSpear::init()
{
	IMAGEMANAGER->addImage("skill_spear", "resource/player/iceSpear.bmp", 130, 130, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("skill_spearX2", "resource/player/iceSpear.bmp", 96 * 2, 96 * 2, true, RGB(255, 0, 255));
	imgRadius = IMAGEMANAGER->findImage("skill_spear")->getWidth() / 2;

	info.keyName = "skill_iceSpear";
	info.explanation = "";
	info.coolTime = 360;

	isCoolTime = false;
	currentCoolTime = 0;

	spearCount = time = 0;
	eTime = sTime = delay = 0;
	upgread = false;
	active = false;

	saveRange = 0;
	return S_OK;
}

void iceSpear::release()
{
}

void iceSpear::update()
{
	time++;

	coolTimeReduction();
	move();

	if (active)fireCount();
	upgradeMove();


	//쿨타임

	if (isCoolTime)
	{
		currentCoolTime++;
		if (currentCoolTime == info.coolTime)
		{
			isCoolTime = false;
			currentCoolTime = 0;
		}
	}

}

void iceSpear::render()
{
	image* img = IMAGEMANAGER->findImage("skill_spear");
	image* big = IMAGEMANAGER->findImage("skill_spearX2");

	int size = vSpear.size();
	for (int i = 0; i < size; i++)
	{
		CAMERAMANAGER->RotateRender(getMemDC(), img, vSpear[i].x, vSpear[i].y, vSpear[i].angle);
		if (INPUT->GetToggleKey('L'))CAMERAMANAGER->Ellipse(getMemDC(), vSpear[i].rc);
	}

	size = vStay.size();
	for (int i = 0; i < size; i++)
	{
		CAMERAMANAGER->RotateRender(getMemDC(), img, vStay[i].x, vStay[i].y, vStay[i].angle);
	}

	size = vUltSpear.size();
	for (int i = 0; i < size; i++)
	{
		if (INPUT->GetToggleKey('L'))CAMERAMANAGER->Ellipse(getMemDC(), vUltSpear[i].rc);

		if (vUltSpear[i].isBig)
		{
			CAMERAMANAGER->RotateRender(getMemDC(), big, vUltSpear[i].x, vUltSpear[i].y, vUltSpear[i].angle);
		}

		else
		{
			CAMERAMANAGER->RotateRender(getMemDC(), img, vUltSpear[i].x, vUltSpear[i].y, vUltSpear[i].angle);
		}
	}
}

float iceSpear::rangeCul(float maxRange, float x, float y, float angle)
{
	if (saveRange > 0)return saveRange;

	tagTile* tile = PLAYERDATA->_getTile();
	vector<int> iWall = PLAYERDATA->getWall();

	for (int i = 0; i < maxRange; i += 32)
	{
		int destX = x + cosf(angle) * imgRadius;
		int destY = y - sinf(angle) * imgRadius;

		float culX = destX + cosf(angle) * i;
		float culY = destY - sinf(angle) * i;

		POINT pt = { (LONG)culX,(LONG)culY };

		int size = iWall.size();
		for (int j = 0; j < size; j++)
		{
			int num = PLAYERDATA->getWall()[j];

			if (PtInRect(&tile[num].rc, pt) == false) continue;

			saveRange = i;
			return i;
		}
	}

	saveRange = maxRange;
	return maxRange;
}

void iceSpear::fire(float x, float y, float angle)
{
	//if (vSpear.size() > 0)return;

	//if (gauge < 0.2f) gauge = 0.2f;

	SOUNDMANAGER->play("iceSpearFire", false,-0.2f);
	
	tagSpear spear;

	spear.x = spear.fireX = x;
	spear.y = spear.fireY = y;
	spear.angle = angle;

	spear.speed = 35.f;
	spear.atkPower = 30.f;

	spear.range = rangeCul(500, x, y, angle);
	spear.rc = RectMakeCenter(spear.x, spear.y, 30, 30);
	spear.collision = false;

	vSpear.push_back(spear);
	isCoolTime = true;
	UI->addCoolTime(info.keyName);
}

void iceSpear::move()
{
	int size = vSpear.size();
	for (int i = 0; i < size; i++)
	{
		vSpear[i].x += cosf(vSpear[i].angle) * vSpear[i].speed;
		vSpear[i].y -= sinf(vSpear[i].angle) * vSpear[i].speed;

		float fixX = vSpear[i].x + cosf(vSpear[i].angle) * imgRadius;
		float fixY = vSpear[i].y - sinf(vSpear[i].angle) * imgRadius;

		vSpear[i].rc = RectMakeCenter(fixX, fixY, 30, 30);

		vSpear[i].distance = getDistance(vSpear[i].fireX, vSpear[i].fireY, vSpear[i].x, vSpear[i].y);

		//삭제
		if (vSpear[i].distance > vSpear[i].range)
		{
			saveRange = 0;
			vSpear.erase(vSpear.begin() + i);
			size = vSpear.size();
		}
		else if (vSpear[i].distance < vSpear[i].range && vSpear[i].collision)
		{
			saveRange = 0;
			CAMERAMANAGER->Shake(20, 20, 2);
			vSpear.erase(vSpear.begin());
			size = vSpear.size();
			SOUNDMANAGER->play("iceSpearATK", false,-0.2f);
		}
		else i++;
	}
}


//upgrade
void iceSpear::upgradefire(float x, float y, float angle)
{
	active = true;
	tagStaySpear spear;
	spear.angle = angle;
	spear.speed = 45.f;

	posX = spear.fireX = x;
	posY = spear.fireY = y;

	spear.x = x + cosf(angle) * 50;
	spear.y = y - sinf(angle) * 50;

	vStay.push_back(spear);

	float tempAngle = angle + (140 * (PI / 180));

	spear.x = x + cosf(tempAngle) * 50;
	spear.y = y - sinf(tempAngle) * 50;

	vStay.push_back(spear);

	tempAngle = angle + (220 * (PI / 180));

	spear.x = x + cosf(tempAngle) * 50;
	spear.y = y - sinf(tempAngle) * 50;

	vStay.push_back(spear);

	tempAngle = angle + (240 * (PI / 180));

	spear.x = x + cosf(tempAngle) * 50;
	spear.y = y - sinf(tempAngle) * 50;

	vStay.push_back(spear);

	tempAngle = angle + (120 * (PI / 180));

	spear.x = x + cosf(tempAngle) * 50;
	spear.y = y - sinf(tempAngle) * 50;

	vStay.push_back(spear);

	spear.x = posX;
	spear.y = posY;

	vStay.push_back(spear);
	isCoolTime = true;
	UI->addCoolTime(info.keyName);
}

void iceSpear::fireCount()
{
	if (vStay.empty()) return;

	vStay[0].x = lerp(vStay[0].x, posX, 0.2f);
	vStay[0].y = lerp(vStay[0].y, posY, 0.2f);

	if (sTime % 10 == 0)
	{
		tagSpear ultSpear;

		if (vStay.size() == 1) ultSpear.isBig = true;
		else ultSpear.isBig = false;

		ultSpear.angle = vStay[0].angle;
		ultSpear.speed = vStay[0].speed;
		ultSpear.x = vStay[0].x;
		ultSpear.y = vStay[0].y;
		ultSpear.fireX = vStay[0].fireX;
		ultSpear.fireY = vStay[0].fireY;
		ultSpear.range = rangeCul(500, ultSpear.fireX, ultSpear.fireY, ultSpear.angle);
		ultSpear.atkPower = 25;
		ultSpear.rc = RectMakeCenter(ultSpear.x, ultSpear.y, 30, 30);
		ultSpear.collision = false;
		SOUNDMANAGER->play("iceSpearFire", false, -0.2f);
		vUltSpear.push_back(ultSpear);
		vStay.erase(vStay.begin());

		sTime = 0;

	}

	sTime++;
}
void iceSpear::upgradeMove()
{
	int size = vUltSpear.size();
	for (int i = 0; i < size;)
	{
		float distance = getDistance(vUltSpear[i].fireX, vUltSpear[i].fireY, vUltSpear[i].x, vUltSpear[i].y);

		if (distance < vUltSpear[i].range && !vUltSpear[i].collision)
		{
			//이동
			vUltSpear[i].x += cosf(vUltSpear[i].angle) * vUltSpear[i].speed;
			vUltSpear[i].y -= sinf(vUltSpear[i].angle) * vUltSpear[i].speed;

			float x = vUltSpear[i].x + cosf(vUltSpear[i].angle) * imgRadius;
			float y = vUltSpear[i].y - sinf(vUltSpear[i].angle) * imgRadius;

			if (vUltSpear[i].isBig)
			{
				vUltSpear[i].rc = RectMakeCenter(x, y, 50, 50);
			}
			else
			{
				vUltSpear[i].rc = RectMakeCenter(x, y, 30, 30);
			}
		}

		if (distance < vUltSpear[i].range && !vUltSpear[i].isBig)
		{
			CAMERAMANAGER->Shake(10, 10, 2);
			i++;
			continue;
		}
		if (vUltSpear[i].isBig)
		{
			//삭제
			if (distance > vUltSpear[i].range)
			{
				eTime++;

				if (eTime > 35)
				{
					vUltSpear.clear();
					active = false;
					eTime = 0;
					saveRange = 0;
					size = vUltSpear.size();
				}
			}
			else if (distance < vUltSpear[i].range && vUltSpear[i].collision)
			{
				eTime++;
				
				if (eTime > 35)
				{
					
					vUltSpear.clear();
					CAMERAMANAGER->Shake(5, 5, 2);
					active = false;
					eTime = 0;
					saveRange = 0;
					size = vUltSpear.size();
				}
			}
		}
		i++;
	}
}
void iceSpear::coolTimeReduction()
{
	if (PLAYERDATA->getStat().CoolTimeReduction != 0)
	{
		info.coolTime = 360 - (PLAYERDATA->getStat().CoolTimeReduction * 60);
		UI->fixCoolTime(info.keyName, info.coolTime);
	}
	else
	{
		info.coolTime = 360;
		UI->fixCoolTime(info.keyName, info.coolTime);
	}
}