#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{

	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small1.bmp", 1000, 2500, 10, 25);
	IMAGEMANAGER->addFrameImage("playerSpearFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);
	IMAGEMANAGER->addFrameImage("PlayerAttackCircle", "resource/player/PlayerAttackCircle1.bmp", 3600, 100, 36, 1);
	IMAGEMANAGER->addFrameImage("flame", "resource/player/flame1.bmp", 4096, 128, 32, 1);
	IMAGEMANAGER->addFrameImage("flameStrike", "resource/player/flameStrike1.bmp", 1707, 171, 10, 1);


	posX = WINSIZEX / 2;
	posY = WINSIZEY / 2;
	rc = RectMakeCenter(posX, posY, 50, 50);


	//enum �ʱ�ȭ
	state = STATE::IDLE;
	move = MOVE::DOWN;

	memset(tileCheck, 0, sizeof(tileCheck));
	memset(diagonalCheck, 0, sizeof(diagonalCheck));

	//colision detection Rect
	makeCol((int)DIRECTION::TOP, 0, -30);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 20);
	makeCol((int)DIRECTION::RIGHT, +45, 20);

	makeCol((int)DIRECTION::LEFT_TOP, -15, -5);
	makeCol((int)DIRECTION::RIGHT_TOP, 15, -5);
	makeCol((int)DIRECTION::LEFT_DOWN, -15, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 15, 35);
	//3��..
	//diagonal collision detection Rect
	makeCol2(0, -45, -55);
	makeCol2(1, 45, -55);
	makeCol2(2, -45, 60);
	makeCol2(3, 45, 60);

	//dash, direction
	dashLeft = dashRight = dashUp = dashDown = false;
	isLeft = isRight = isUp = isDown = false;
	//attack type
	basic = standard = signature = signatureE = signatureR = false;

	speed = 0;

	//animation count ,index
	atkCount = atkIndex = index = dashIndex = count = dashCount = basicCount = basicIndex = 0;
	basicStateCool = infernoStateCool = signatureStateCool = 0;
	spearIndex = 0;
	idleDelay = 0;



	//blaze 3�� �߻� �뵵
	blazeCount = 0;

	//�ҷ� Ŭ����
	bulletClassInit();

	//skill init
	skillInit();

	//�κ��丮
	inven = new inventory;
	inven->init();

	//angle between mouse & player
	attackAngle = saveAngle = 0;
	angleTenth = 0;

	//damage 
	frozenTime = grabbedTime = 0;

	isGrabbed = false;
	isDamaged = false;
	isDead = false;

	//upgrade gauge
	skillGauge = 0;
	gaugeMaxCool = 120;
	upgradeReady = false;


	//sound
	walkCount = 0;

	return S_OK;
}

void player::release()
{
	blaze->release();
	SAFE_DELETE(blaze);
	dragon->release();
	SAFE_DELETE(dragon);
	Meteor->release();
	SAFE_DELETE(Meteor);
	searingRush->release();
	SAFE_DELETE(searingRush);
	inferno->release();
	SAFE_DELETE(inferno);
	spear->release();
	SAFE_DELETE(spear);

	inven->release();
	SAFE_DELETE(inven);
}

void player::update()
{
	PLAYERDATA->update();

	blaze->update();
	dragon->update();
	Meteor->update();
	searingRush->update();

	inferno->update();
	spear->update();

	inven->update();

	//animation count
	count++;
	dashCount++;
	atkCount++;
	basicCount++;
	idleDelayCount();

	// angle(mouse-player), angleTenth
	attackAngle = getAngle(posX, posY, CAMERAMANAGER->GetAbsoluteX(_ptMouse.x), CAMERAMANAGER->GetAbsoluteY(_ptMouse.y));
	angleTenth = (int)(saveAngle * (18 / PI));


	//�÷��̾� ��Ʈ�� ����
	if (speed == 0 && basicStateCool == 0 && signatureStateCool == 0 && frozenTime == 0
		&& !isDead && !inferno->getGauging() && idleDelay == 0)
	{
		controller();
	}

	////////////////////////////
	//  collision dectection  //
	tileCol();

	makeCol((int)DIRECTION::TOP, 0, -30);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 20);
	makeCol((int)DIRECTION::RIGHT, +45, 20);

	makeCol((int)DIRECTION::LEFT_TOP, -15, -5);
	makeCol((int)DIRECTION::RIGHT_TOP, 15, -5);
	makeCol((int)DIRECTION::LEFT_DOWN, -15, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 15, 35);

	makeCol2(0, -45, -55);
	makeCol2(1, 45, -55);
	makeCol2(2, -45, 60);
	makeCol2(3, 45, 60);
	////////////////////////////

	changeState();

	////////////////
	basicSetUp();
	dashSetUp();
	standardSetUp();
	signatureSetUp();
	signatureSetUpE();
	signatureSetUpR();
	/////////////////


	damagedCool();
	// knockBack lerp
	if (isDamaged)
	{
		int index = 0;
		for (int i = 0; i < 4; i++) index = i;

		if (tileCheck[index].isCol || tileCheck[index + 4].isCol || diagonalCheck[index].isCol)return;
		knockBack.percent -= 0.2f;
		posX += cosf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		posY -= sinf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		rc = RectMakeCenter(posX, posY, 50, 50);
	}

	skillGaugeSetUp();

	takeCoin();
	takeHealball();

	// camera�� ���󰡴� ���
	CAMERAMANAGER->MovePivot(posX, posY);
	death();
	//don't touch!
	buttonDown();

}

void player::other_update()
{
	PLAYERDATA->update();

	blaze->update();
	dragon->update();
	Meteor->update();
	searingRush->update();

	inferno->update();
	spear->update();
	idleDelayCount();

	inven->update();

	//animation count
	count++;
	dashCount++;
	atkCount++;
	basicCount++;

	// angle(mouse-player), angleTenth
	attackAngle = getAngle(posX, posY, CAMERAMANAGER->GetAbsoluteX(_ptMouse.x), CAMERAMANAGER->GetAbsoluteY(_ptMouse.y));
	angleTenth = (int)(saveAngle * (18 / PI));


	if (speed == 0 && basicStateCool == 0 && signatureStateCool == 0 && frozenTime == 0
		&& !isDead && !inferno->getGauging() && !isGrabbed && idleDelay == 0)
	{
		controller();
	}

	makeCol((int)DIRECTION::TOP, 0, -30);
	makeCol((int)DIRECTION::BOTTOM, 0, 60);
	makeCol((int)DIRECTION::LEFT, -45, 0);
	makeCol((int)DIRECTION::RIGHT, +45, 0);

	makeCol((int)DIRECTION::LEFT_TOP, -20, -30);
	makeCol((int)DIRECTION::RIGHT_TOP, 25, -30);
	makeCol((int)DIRECTION::LEFT_DOWN, -20, 35);
	makeCol((int)DIRECTION::RIGHT_DOWN, 25, 35);

	makeCol2(0, -45, -55);
	makeCol2(1, 45, -55);
	makeCol2(2, -45, 60);
	makeCol2(3, 45, 60);


	changeState();

	////////////////
	basicSetUp();
	dashSetUp();
	standardSetUp();
	signatureSetUp();
	signatureSetUpE();
	signatureSetUpR();
	/////////////////



	damagedCool();
	grabbedCool();

	// knockBack lerp
	if (isDamaged)
	{
		if (tileCheck[index].isCol || tileCheck[index + 4].isCol || diagonalCheck[index].isCol)return;
		knockBack.percent -= 0.2f;
		posX += cosf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		posY -= sinf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		rc = RectMakeCenter(posX, posY, 50, 50);
	}



	skillGaugeSetUp();


	takeCoin();
	takeHealball();

	// camera�� ���󰡴� ���
	CAMERAMANAGER->MovePivot(posX, posY);
	death();

	//don't touch!
	buttonDown();

}

void player::render(int _index)
{
	int tempAngle = attackAngle * (18 / PI);
	image* img = IMAGEMANAGER->findImage("PlayerAttackCircle");
	CAMERAMANAGER->AlphaFrameRender(getMemDC(), img, posX - 50, posY - 20, tempAngle, 0, 50);


	if (_index == 0)
	{
		//bool isRender = false;

	// DASH FIRE RENDER
		for (int i = 0; i < searingRush->getSize(); i++)
		{
			if (/*!isRender && */(searingRush->getY(i) > posY))
			{
				//isRender = true;
				searingRush->singleRender(i);
			}

			else searingRush->singleRender(i);
		}

		//if (!isRender)animation(); // z���� ���� ������.. (���߿� frame�� �������� �����ϴ� ���..)
	}

	else
	{
		for (int i = 0; i < searingRush->getSize(); i++)
		{
			if (searingRush->getY(i) > posY)
			{
				//animation(1);
				searingRush->singleRender(i);
			}
			else searingRush->singleRender(i);
		}

		animation(1);
	}

	blaze->render();
	dragon->render();
	Meteor->render();
	inferno->render();
	spear->render();

	if (INPUT->GetToggleKey('L'))
	{
		for (int i = 0; i < 4; i++)
		{
			CAMERAMANAGER->Rectangle(getMemDC(), tileCheck[i].rc);
			CAMERAMANAGER->Rectangle(getMemDC(), tileCheck[i + 4].rc);
			CAMERAMANAGER->Rectangle(getMemDC(), diagonalCheck[i].rc);
		}
	}

	viewText();
}

void player::invenRender()
{
	inven->render();
}

void player::bulletClassInit()
{
	blaze = new bomb;
	blaze->init(10);

	dragon = new dragonArc;
	dragon->init();

	Meteor = new meteor;
	Meteor->init();

	searingRush = new dashFire;
	searingRush->init();

	inferno = new RagingInferno;
	inferno->init();

	spear = new iceSpear;
	spear->init();
}

void player::controller()
{
	//����
	if (isLeft)
	{
		rc = RectMakeCenter(posX, posY, 50, 50);
		if (!tileCheck[(int)DIRECTION::LEFT].isCol)
			posX -= 8;
		walkCount++;
		if (walkCount == 16)
		{
			SOUNDMANAGER->play("playerFoot", false);
			walkCount = 0;
		}
	}
	//������
	if (isRight)
	{
		rc = RectMakeCenter(posX, posY, 50, 50);
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol)
			posX += 8;
		walkCount++;
		if (walkCount == 16)
		{
			SOUNDMANAGER->play("playerFoot", false);
			walkCount = 0;
		}
	}
	//��
	if (isUp)
	{
		rc = RectMakeCenter(posX, posY, 50, 50);
		if (!tileCheck[(int)DIRECTION::LEFT_TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol)
			posY -= 8;
		walkCount++;
		if (walkCount == 16)
		{
			SOUNDMANAGER->play("playerFoot", false);
			walkCount = 0;
		}
	}
	//�Ʒ�
	if (isDown)
	{
		rc = RectMakeCenter(posX, posY, 50, 50);
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol && !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol)
			posY += 8;
		walkCount++;
		if (walkCount == 16)
		{
			SOUNDMANAGER->play("playerFoot", false);
			walkCount = 0;
		}
	}

	//DASH
	if (INPUT->GetKeyDown(VK_SPACE) && speed == 0)
	{
		//dash = true;
		//�뽬 ����Ʈ ����
		EFFECT->dashEffect(move, { (long)posX,(long)posY });
		SOUNDMANAGER->play("playerNomalDash", false);
		state = STATE::DASH;

		switch (move)
		{
		case MOVE::LEFT:
			dashLeft = true;
			break;
		case MOVE::LEFT_TOP:
			dashLeft = true;
			dashUp = true;
			break;
		case MOVE::RIGHT:
			dashRight = true;
			break;
		case MOVE::RIGHT_TOP:
			dashRight = true;
			dashUp = true;
			break;
		case MOVE::UP:
			dashUp = true;
			break;
		case MOVE::DOWN:
			dashDown = true;
			break;
		case MOVE::LEFT_DOWN:
			dashLeft = true;
			dashDown = true;
			break;
		case MOVE::RIGHT_DOWN:
			dashRight = true;
			dashDown = true;
			break;
		default:
			break;
		}
		speed = 20;
	}
}

void player::skillInit()
{
	arcana[0].type = ARCANA_TYPE::TYPE_BASIC;
	arcana[0].skillName = blaze->getInfo().keyName;
	arcana[0].explanation = blaze->getInfo().explanation;
	arcana[0].coolTime = blaze->getInfo().coolTime;

	arcana[1].type = ARCANA_TYPE::TYPE_DASH;
	arcana[1].skillName = searingRush->getInfo().keyName;
	arcana[1].explanation = searingRush->getInfo().explanation;
	arcana[1].coolTime = searingRush->getInfo().coolTime;

	arcana[2].type = ARCANA_TYPE::TYPE_STANDARD;
	arcana[2].skillName = inferno->getInfo().keyName;
	arcana[2].explanation = inferno->getInfo().explanation;
	arcana[2].coolTime = inferno->getInfo().coolTime;

	arcana[3].type = ARCANA_TYPE::TYPE_SIGNATURE;
	arcana[3].skillName = Meteor->getInfo().keyName;
	arcana[3].explanation = Meteor->getInfo().explanation;
	arcana[3].coolTime = Meteor->getInfo().coolTime;

	arcana[4].type = ARCANA_TYPE::TYPE_SIGNATURE;
	arcana[4].skillName = dragon->getInfo().keyName;
	arcana[4].explanation = dragon->getInfo().explanation;
	arcana[4].coolTime = dragon->getInfo().coolTime;


	arcana[5].type = ARCANA_TYPE::TYPE_SIGNATURE;
	arcana[5].skillName = spear->getInfo().keyName;
	arcana[5].explanation = spear->getInfo().explanation;
	arcana[5].coolTime = spear->getInfo().coolTime;


	for (int i = 0; i < ARCANA_SLOT; i++)
	{
		UI->setSkillSlot(arcana[i].skillName, arcana[i].coolTime);
		PLAYERDATA->pushArcanaInfo(arcana[i]);
	}
}

void player::setSkillUi(ARCANA_TYPE type, string keyName, int coolTime)
{

	if (keyName != "")
	{
		for (int i = 0; i < ARCANA_SLOT; i++)
		{
			if (type == ARCANA_TYPE::TYPE_BASIC) i = 0;
			else if (type == ARCANA_TYPE::TYPE_DASH) i = 1;
			else if (type == ARCANA_TYPE::TYPE_STANDARD) i = 2;
			else if (type == ARCANA_TYPE::TYPE_SIGNATURE) i = 3;


			arcana[i].skillName = keyName;
			arcana[i].coolTime = coolTime;

			UI->setSkillSlot(arcana[i].skillName, arcana[i].coolTime);
			PLAYERDATA->setArcanaInfo(i, arcana[i].skillName, arcana[i].explanation, coolTime);

		}
	}
}

void player::basicSetUp()
{
	if (arcana[0].skillName == blaze->getInfo().keyName)
	{
		if (INPUT->GetKeyDown(VK_LBUTTON) && !blaze->getCool() && !isDead && speed == 0 && !inferno->getGauging()
			&& frozenTime == 0)
		{
			saveAngle = attackAngle;
			basic = true;
		}

		if (basic)
		{
			float x = cosf(saveAngle) * 50.f + posX;
			float y = -sinf(saveAngle) * 50 + posY;

			if (basicStateCool == 0)
			{
				basicStateCool = 30;
				blaze->fire(x, y, 10, saveAngle);
			}

			if (basicStateCool > 0)
			{
				state = STATE::BASIC;
				basicStateCool--;
			}
			if (basicStateCool == 0) basic = false;
		}

	}
}

void player::dashSetUp()
{
	if (speed == 0 || isDead) return;

	//searing dash fire
	if (arcana[1].skillName == "skill_searingDash")
	{
		if (!searingRush->getIsCoolTime() &&
			(speed == 8 || speed == 11 || speed == 14 || speed == 17))

			searingRush->fire(posX, posY);

		if (!searingRush->getIsCoolTime() && speed == 17) //sound
			SOUNDMANAGER->play("playerfireDash", false);

		if (speed == 7)
		{
			UI->addCoolTime("skill_searingDash");
			searingRush->setIsCoolTime(true);
		}

	}

	//moving
	if (dashLeft)
	{
		if (dashUp)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_TOP].isCol &&
				!tileCheck[(int)DIRECTION::TOP].isCol && !diagonalCheck[0].isCol)
			{
				posX -= speed;
				posY -= speed;
				OffsetRect(&rc, -speed, -speed);
				RectMakeCenter(posX, posY, 50, 50);
			}
		}
		else if (dashDown)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::BOTTOM].isCol &&
				!diagonalCheck[2].isCol)
			{
				posX -= speed;
				posY += speed;
				OffsetRect(&rc, -speed, speed);
				RectMakeCenter(posX, posY, 50, 50);
			}
		}
		else // �׳� ���� LEFT
		{
			if (!tileCheck[(int)DIRECTION::LEFT].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol)

			{
				posX -= speed;
				OffsetRect(&rc, -speed, 0);
				RectMakeCenter(posX, posY, 50, 50);
			}
		}
	}

	else if (dashRight)
	{
		if (dashUp)
		{
			if (!tileCheck[(int)DIRECTION::RIGHT_TOP].isCol &&
				!diagonalCheck[1].isCol && !diagonalCheck[2].isCol)
			{
				posX += speed;
				posY -= speed;
				OffsetRect(&rc, speed, -speed);
				RectMakeCenter(posX, posY, 50, 50);
			}
		}
		else if (dashDown)
		{
			if (!tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol &&
				!diagonalCheck[3].isCol)
			{
				posX += speed;
				posY += speed;
				OffsetRect(&rc, speed, speed);
				RectMakeCenter(posX, posY, 50, 50);

			}
		}
		else // �׳� ���� RIGHT
		{
			if (!tileCheck[(int)DIRECTION::RIGHT].isCol)
			{
				posX += speed;
				OffsetRect(&rc, speed, 0);
				RectMakeCenter(posX, posY, 50, 50);
			}
		}
	}

	else if (dashUp)
	{
		//���� UP
		if (!tileCheck[(int)DIRECTION::TOP].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol
			&& !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol)
		{
			posY -= speed;
			OffsetRect(&rc, 0, -speed);
			RectMakeCenter(posX, posY, 50, 50);
		}
	}

	else if (dashDown)
	{
		//���� DOWN
		if (!tileCheck[(int)DIRECTION::BOTTOM].isCol && !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol
			&& !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol)
		{
			posY += speed;
			OffsetRect(&rc, 0, speed);
			RectMakeCenter(posX, posY, 50, 50);
		}
	}
	speed--;
	if (speed == 0) resetKey();

}

void player::standardSetUp()
{
	if (arcana[2].skillName == inferno->getInfo().keyName)
	{
		if (INPUT->GetKeyDown(VK_RBUTTON) && frozenTime == 0 && !inferno->getGauging() && !isDead
			&& speed == 0 && signatureStateCool == 0 && !inferno->getCool() && !isGrabbed)
		{
			standard = true;
			saveAngle = attackAngle;
			inferno->fire(posX, posY, attackAngle);

		}
		if (INPUT->GetKeyUp(VK_RBUTTON)) standard = false;


		if (inferno->getGauging())
		{
			state = STATE::STANDARD;
		}
	}
}

void player::signatureSetUp()
{
	Meteor->setUpgrade(upgradeReady);
	dragon->setUpgrade(upgradeReady);

	float mouseX = CAMERAMANAGER->GetAbsoluteX(_ptMouse.x);
	float mouseY = CAMERAMANAGER->GetAbsoluteY(_ptMouse.y);

	if (INPUT->GetKeyDown('Q') && frozenTime == 0 && !isDead && !isGrabbed && !inferno->getGauging() && speed == 0
		&& !Meteor->getCool() && !dragon->getCool() && signatureStateCool == 0)
	{
		signature = true;
	}

	if (signature)
	{
		if (!upgradeReady)
		{
			if (arcana[3].skillName == Meteor->getInfo().keyName)
			{

				if (signatureStateCool == 0)
				{
					signatureStateCool = 30;
					Meteor->creatMeteor(mouseX, mouseY, 0);
				}

				if (signatureStateCool > 0)
				{
					state = STATE::SIGNATURE;
					signatureStateCool--;

				}
				if (signatureStateCool == 0) signature = false;
			}

			else if (arcana[3].skillName == dragon->getInfo().keyName)
			{
				if (signatureStateCool == 0)
				{
					signatureStateCool = 30;
					dragon->fire(posX, posY, attackAngle);

				}
				if (signatureStateCool > 0)
				{
					state = STATE::SIGNATURE;
					signatureStateCool--;
				}
				if (signatureStateCool == 0) signature = false;
			}
		}
		else
		{
			TIME->setTest(12.f);
			EFFECT->ultEftPlay({ (long)posX,(long)posY }, 10);
			if (arcana[3].skillName == "skill_meteor")
			{

				if (signatureStateCool == 0)
				{
					signatureStateCool = 30;
					Meteor->creatMeteor(mouseX, mouseY, 0);

					//skillGauge �ʱ�ȭ
					skillGauge = 0;
				}
				if (signatureStateCool >= 0)
				{
					state = STATE::SIGNATURE;
					signatureStateCool--;

					if (signatureStateCool == 0) signature = false;
				}

			}
			else if (arcana[3].skillName == "skill_dragonArc")
			{
				TIME->setTest(12.f);
				EFFECT->ultEftPlay({ (long)posX,(long)posY }, 10);
				if (signatureStateCool == 0)
				{
					signatureStateCool = 30;
					dragon->phoenixFire(posX, posY, attackAngle);
					skillGauge = 0;
				}
				if (signatureStateCool >= 0)
				{
					state = STATE::SIGNATURE;
					signatureStateCool--;
					if (signatureStateCool == 0) signature = false;

				}
			}
		}
	}
}

void player::signatureSetUpE()
{
	// arcana[4] , key -> E
	if (arcana[4].skillName == spear->getInfo().keyName)
	{
		if (!isDead && !inferno->getGauging() && frozenTime == 0 && !isGrabbed && !spear->getCool())
		{
			if (!upgradeReady)
			{
				if (INPUT->GetKey('E'))
				{
					spear->chargeSpear();
					spear->setGauging(true);
				}

				if (INPUT->GetKeyUp('E'))
				{
					spear->fire(posX, posY, attackAngle);
				}
			}
			else
			{
				TIME->setTest(12.f);
				EFFECT->ultEftPlay({ (long)posX,(long)posY }, 10);
				if (INPUT->GetKeyDown('E'))
				{
					spear->upgradefire(posX, posY, attackAngle);
					skillGauge = 0;
				}

			}
		}
	}
	else if (arcana[4].skillName == dragon->getInfo().keyName)
	{
		if (!isDead && !inferno->getGauging() && frozenTime == 0 && !spear->getCool())
		{
			if (!upgradeReady)
			{
			}
			else
			{
				TIME->setTest(12.f);
				EFFECT->ultEftPlay({ (long)posX,(long)posY }, 10);
			}
		}
	}

}

void player::signatureSetUpR()
{
	// arcana[5], key->R
	if (arcana[5].skillName == spear->getInfo().keyName)
	{
		if (!isDead && !inferno->getGauging() && frozenTime == 0 && !spear->getCool())
		{

			if (!upgradeReady)
			{
				if (INPUT->GetKey('R'))
				{
					spear->chargeSpear();
				}
				if (INPUT->GetKeyUp('R'))
				{
					spear->fire(posX, posY, attackAngle);
				}
			}
			else
			{

				if (INPUT->GetKeyDown('R'))
				{
					spear->upgradefire(posX, posY, attackAngle);
					skillGauge = 0;
				}

			}
		}
	}
	else if (arcana[5].skillName == dragon->getInfo().keyName)
	{

	}

}
void player::takeCoin()
{
	for (int i = 0; i < DROP->getCoinVec().size(); i++)
	{
		if (colCheck(rc, DROP->getCoinRect(i)))
		{
			PLAYERDATA->setCoin(PLAYERDATA->getCoin() + DROP->getCoinVec()[i].money);
			DROP->getCoinEffect(DROP->getCoinVec()[i].money);
			if (PLAYERDATA->getStat().goldPig)PLAYERDATA->setCoin(PLAYERDATA->getCoin() + RANDOM->range(1, 2));

			DROP->delCoin(i);
			SOUNDMANAGER->play("coinGet", false, 0.13f);
		}
	}
}

void player::takeHealball()
{
	for (int i = 0; i < DROP->getBallVec().size(); i++)
	{
		if (colCheck(rc, DROP->getHealRect(i)))
		{
			PLAYERDATA->setHp(PLAYERDATA->getHp() + DROP->getBallVec()[i].heal);
			PARTICLE->potionParticlePlay(posX, posY);
			DROP->delHeal(i);
			SOUNDMANAGER->play("healBall", false);
		}
	}
}

void player::animation(int _index)
{
	int damageAngleTenth = (int)(knockBack.angle * (18 / PI));

	if (_index == 0)
	{
		switch (state)
		{
		case STATE::IDLE:

			if (move == MOVE::LEFT)frameAnimation(3, 0);
			else if (move == MOVE::RIGHT) frameAnimation(2, 0);
			else if (move == MOVE::UP) frameAnimation(1, 0);
			else frameAnimation(0, 0);

			break;
		case STATE::RUN:
			if (move == MOVE::LEFT || move == MOVE::LEFT_TOP || move == MOVE::LEFT_DOWN)
			{
				if (count % 6 == 0)
				{
					index--;
					if (index < 0)index = 4;
				}
				frameAnimation(index, 4);
			}
			else if (move == MOVE::RIGHT || move == MOVE::RIGHT_TOP || move == MOVE::RIGHT_DOWN)
			{
				if (count % 6 == 0)
				{
					index++;
					if (index > 4) index = 0;
				}
				frameAnimation(index, 3);
			}
			else if (move == MOVE::UP)
			{
				if (count % 5 == 0)
				{
					index++;
					if (index > 9)index = 0;
				}
				frameAnimation(index, 2);
			}
			else if (move == MOVE::DOWN)
			{
				if (count % 5 == 0)
				{
					index++;
					if (index > 9)index = 0;
				}
				frameAnimation(index, 1);
			}
			break;
		case STATE::DASH:

			if (dashLeft)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0) dashIndex = 0;
				}
				frameAnimation(dashIndex, 8);
			}
			else if (dashRight)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0)dashIndex = 0;
				}
				frameAnimation(dashIndex, 7);
			}
			else if (dashUp)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0)dashIndex = 0;
				}
				frameAnimation(dashIndex, 18);
			}
			else if (dashDown)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0)dashIndex = 0;
				}
				frameAnimation(dashIndex, 19);
			}
			break;
		case STATE::DAMAGED:

			//�������� 

			if (damageAngleTenth > 14 && damageAngleTenth <= 23)// ���ݹ��� �����ʿ��� �����̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(7, 0);
					if (!isDamaged || !isGrabbed) move = MOVE::RIGHT;

				}
			}
			else if (damageAngleTenth <= 4 && damageAngleTenth >= 0 || damageAngleTenth > 32) //���ݹ��� ���ʿ��� �������̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(6, 0);
					if (!isDamaged || !isGrabbed) move = MOVE::LEFT;

				}
			}

			else if (damageAngleTenth > 4 && damageAngleTenth <= 14) //���ݹ��� �Ʒ��ʿ��� �����̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(4, 0);
					if (!isDamaged || !isGrabbed) move = MOVE::UP;

				}
			}
			else if (damageAngleTenth > 23 && damageAngleTenth <= 32)//���ݹ��� ���ʿ��� �Ʒ����̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(5, 0);
					if (!isDamaged || !isGrabbed) move = MOVE::DOWN;

				}
			}
			break;
		case STATE::DIE:
			if (index < 5 && count % 30 == 0)
				index++;

			if (index >= 5 && count % 7 == 0)
				index++;

			if (index > 9) index = 9;
			frameAnimation(index, 9);
			break;
		case STATE::BASIC:

			if (angleTenth > 14 && angleTenth <= 23)//left
			{ //15 16 17 18 19 20 21 22 23
				if (basicCount % 5 == 0)
				{
					basicIndex++;
					if (basicIndex > 6 || basicStateCool == 0) basicIndex = 0;
				}
				frameAnimation(basicIndex, 5);
				//���� ���� ������ ���� ���ϱ�
				if (basicStateCool == 0)
					move = MOVE::LEFT;

			}
			else if (angleTenth <= 4 && angleTenth >= 0 || angleTenth > 32) //right
			{ // 0 1 2 3 4 , 33 34 35

				if (basicCount % 5 == 0)
				{
					basicIndex++;

					if (basicIndex > 6 || basicStateCool == 0) basicIndex = 0;
				}
				frameAnimation(basicIndex, 6);

				// ������ ���� ������ ������ ���ϱ�
				if (basicStateCool == 0)
					move = MOVE::RIGHT;

			}
			else if (angleTenth > 4 && angleTenth <= 14) //up
			{//5 6 7 8 9 10 11 12 13 14
				if (basicCount % 5 == 0)
				{
					basicIndex++;
					if (basicIndex > 6 || basicStateCool == 0)basicIndex = 0;
				}
				frameAnimation(basicIndex, 14);
				//���� ���� ������ ���� ���ϱ�
				if (basicStateCool == 0)
					move = MOVE::UP;
			}
			else if (angleTenth > 23 && angleTenth <= 32) //down
			{ //24 25 26 27 28 29 30 31 32
				if (basicCount % 5 == 0)
				{
					basicIndex++;
					if (basicIndex > 6 || basicStateCool == 0)basicIndex = 0;
				}
				frameAnimation(basicIndex, 6);
				//�Ʒ��� ���� ������ �Ʒ��� ���ϱ�
				if (basicStateCool == 0)
					move = MOVE::DOWN;
			}
			break;
		case STATE::STANDARD:

			if (angleTenth > 14 && angleTenth <= 23)
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}
				frameAnimation(atkIndex, 6);


				//���� ���� ������ ���� ���ϱ�
				if (gaugeTime > 50) move = MOVE::LEFT;
			}
			else if (angleTenth <= 4 && angleTenth >= 0 || angleTenth > 32) //right
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}

				frameAnimation(atkIndex, 5);
				// ������ ���� ������ ������ ���ϱ�
				if (gaugeTime > 50) move = MOVE::RIGHT;
			}
			else if (angleTenth > 4 && angleTenth <= 14) //up
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}
				frameAnimation(atkIndex, 14);
				//���� ���� ������ ���� ���ϱ�
				if (gaugeTime > 50) move = MOVE::UP;
			}
			else if (angleTenth > 23 && angleTenth <= 32) //down
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}
				frameAnimation(atkIndex, 6);
				//�Ʒ��� ���� ������ �Ʒ��� ���ϱ�
				if (gaugeTime > 50) move = MOVE::DOWN;
			}

			break;
		case STATE::SIGNATURE:

			if (move == MOVE::LEFT)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 10);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 16);

			}
			else if (move == MOVE::RIGHT)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 20);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 15);

			}
			else if (move == MOVE::UP || move == MOVE::LEFT_TOP || move == MOVE::RIGHT_TOP)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 20);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 14);
				
			}
			else if (move == MOVE::DOWN || move == MOVE::LEFT_DOWN || move == MOVE::RIGHT_DOWN)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 10);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 6);
				
				break;
			}
		}
	}
	else
	{
		switch (state)
		{
		case STATE::IDLE:

			if (move == MOVE::LEFT)frameAnimation(3, 0, 1);
			else if (move == MOVE::RIGHT) frameAnimation(2, 0, 1);
			else if (move == MOVE::UP) frameAnimation(1, 0, 1);
			else frameAnimation(0, 0, 1);

			break;
		case STATE::RUN:
			if (move == MOVE::LEFT || move == MOVE::LEFT_TOP || move == MOVE::LEFT_DOWN)
			{
				if (count % 6 == 0)
				{
					index--;
					if (index < 0)index = 4;
				}
				frameAnimation(index, 4, 1);
			}
			else if (move == MOVE::RIGHT || move == MOVE::RIGHT_TOP || move == MOVE::RIGHT_DOWN)
			{
				if (count % 6 == 0)
				{
					index++;
					if (index > 4) index = 0;
				}
				frameAnimation(index, 3, 1);
			}
			else if (move == MOVE::UP)
			{
				if (count % 5 == 0)
				{
					index++;
					if (index > 9)index = 0;
				}
				frameAnimation(index, 2, 1);
			}
			else if (move == MOVE::DOWN)
			{
				if (count % 5 == 0)
				{
					index++;
					if (index > 9)index = 0;
				}
				frameAnimation(index, 1, 1);
			}
			break;
		case STATE::DASH:
			if (dashLeft)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0) dashIndex = 0;
				}
				frameAnimation(dashIndex, 8, 1);
			}
			else if (dashRight)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0)dashIndex = 0;
				}
				frameAnimation(dashIndex, 7, 1);
			}
			else if (dashUp)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0)dashIndex = 0;
				}
				frameAnimation(dashIndex, 18, 1);
			}
			else if (dashDown)
			{
				if (dashCount % 3 == 0)
				{
					dashIndex++;
					if (dashIndex > 9 || speed == 0)dashIndex = 0;
				}
				frameAnimation(dashIndex, 19, 1);
			}
			break;
		case STATE::DAMAGED:
			//�������� 

			if (damageAngleTenth > 14 && damageAngleTenth <= 23)// ���ݹ��� �����ʿ��� �����̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(7, 0, 1);
					if (!isDamaged || !isGrabbed) move = MOVE::RIGHT;

				}
			}
			else if (damageAngleTenth <= 4 && damageAngleTenth >= 0 || damageAngleTenth > 32) //���ݹ��� ���ʿ��� �������̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(6, 0, 1);
					if (!isDamaged || !isGrabbed) move = MOVE::LEFT;

				}
			}

			else if (damageAngleTenth > 4 && damageAngleTenth <= 14) //���ݹ��� �Ʒ��ʿ��� �����̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(4, 0, 1);
					if (!isDamaged || !isGrabbed) move = MOVE::UP;

				}
			}
			else if (damageAngleTenth > 23 && damageAngleTenth <= 32)//���ݹ��� ���ʿ��� �Ʒ����̸�
			{
				if (frozenTime > 0 || grabbedTime > 0)
				{
					frameAnimation(5, 0, 1);
					if (!isDamaged || !isGrabbed) move = MOVE::DOWN;

				}
			}
			break;
		case STATE::DIE:
			if (index < 5 && count % 30 == 0)
				index++;

			if (index >= 5 && count % 7 == 0)
				index++;

			if (index > 9) index = 9;
			frameAnimation(index, 9, 1);
			break;
		case STATE::BASIC:

			if (angleTenth > 14 && angleTenth <= 23)//left
			{ //15 16 17 18 19 20 21 22 23
				if (basicCount % 5 == 0)
				{
					basicIndex++;
					if (basicIndex > 6 || basicStateCool == 0) basicIndex = 0;

				}
				frameAnimation(basicIndex, 5, 1);
				//���� ���� ������ ���� ���ϱ�
				if (basicStateCool == 0)

					move = MOVE::LEFT;

			}
			else if (angleTenth <= 4 && angleTenth >= 0 || angleTenth > 32) //right
			{ // 0 1 2 3 4 , 33 34 35

				if (basicCount % 5 == 0)
				{
					basicIndex++;
					if (basicIndex > 6 || basicStateCool == 0) basicIndex = 0;
				}
				frameAnimation(basicIndex, 6, 1);

				// ������ ���� ������ ������ ���ϱ�
				if (basicStateCool == 0)

					move = MOVE::RIGHT;

			}
			else if (angleTenth > 4 && angleTenth <= 14) //up
			{//5 6 7 8 9 10 11 12 13 14
				if (basicCount % 5 == 0)
				{
					basicIndex++;
					if (basicIndex > 6 || basicStateCool == 0)basicIndex = 0;
				}
				frameAnimation(basicIndex, 14, 1);
				//���� ���� ������ ���� ���ϱ�
				if (basicStateCool == 0)
					move = MOVE::UP;
			}
			else if (angleTenth > 23 && angleTenth <= 32) //down
			{ //24 25 26 27 28 29 30 31 32
				if (basicCount % 5 == 0)
				{
					basicIndex++;
					if (basicIndex > 6 || basicStateCool == 0)basicIndex = 0;
				}
				frameAnimation(basicIndex, 6, 1);
				//�Ʒ��� ���� ������ �Ʒ��� ���ϱ�
				if (basicStateCool == 0)
					move = MOVE::DOWN;
			}
			break;
		case STATE::STANDARD:

			if (angleTenth > 14 && angleTenth <= 23)
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}
				frameAnimation(atkIndex, 6, 1);


				//���� ���� ������ ���� ���ϱ�
				if (gaugeTime > 50) move = MOVE::LEFT;
			}
			else if (angleTenth <= 4 && angleTenth >= 0 || angleTenth > 32) //right
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}

				frameAnimation(atkIndex, 5, 1);
				// ������ ���� ������ ������ ���ϱ�
				if (gaugeTime > 50) move = MOVE::RIGHT;
			}
			else if (angleTenth > 4 && angleTenth <= 14) //up
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}
				frameAnimation(atkIndex, 14, 1);
				//���� ���� ������ ���� ���ϱ�
				if (gaugeTime > 50) move = MOVE::UP;
			}
			else if (angleTenth > 23 && angleTenth <= 32) //down
			{
				if (inferno->getGauging()/*gaugeTime < 45*/)atkIndex = 0;
				else
				{
					atkIndex++;
					if (atkIndex > 3) atkIndex = 3;
				}
				frameAnimation(atkIndex, 6, 1);
				//�Ʒ��� ���� ������ �Ʒ��� ���ϱ�
				if (gaugeTime > 50) move = MOVE::DOWN;
			}

			break;
		case STATE::SIGNATURE:

			if (move == MOVE::LEFT)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 10, 1);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 16, 1);
			}
			else if (move == MOVE::RIGHT)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 20, 1);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 15, 1);
			}
			else if (move == MOVE::UP || move == MOVE::LEFT_TOP || move == MOVE::RIGHT_TOP)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 20, 1);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 14, 1);

			}
			else if (move == MOVE::DOWN || move == MOVE::LEFT_DOWN || move == MOVE::RIGHT_DOWN)
			{
				if (atkCount % 10 == 0)
				{
					atkIndex++;
					if (atkIndex > 9)atkIndex = 0;
				}
				if (arcana[3].skillName == "skill_meteor")
					frameAnimation(atkIndex, 10, 1);
				else if (arcana[3].skillName == "skill_dragonArc")
					frameAnimation(atkIndex, 6, 1);
			}
			break;
		} //end of switch
	}

	//if (arcana[3].skillName == spear->getInfo().keyName)
	//{
	//	if (spear->getGauging())
	//		spearIndex = 0;
	//	else if (!spear->getGauging() && count % 5 == 0 && spearIndex < 4)
	//		spearIndex++;



	//	//CAMERAMANAGER->FrameRender(getMemDC(),IMAGEMANAGER->findImage("playerSpearFrame"),posX-50,posY-50,spearIndex
	//}



	}

	void player::frameAnimation(int frameX, int frameY, int _index)
	{
		if (idleDelay > 0)
		{
			if (idleDelay > 20)
			{

				UNITRENDER->setFramePlayer({ 8,0 });
				return;
			}
			else if (idleDelay == 16)	SOUNDMANAGER->play("portalOUT", false, 0.2f);


			if (idleDelay == 1)
			{

				UNITRENDER->setFramePlayer({ 0,0 });
			}
		}

		if (_index == 0)
		{
			UNITRENDER->setFramePlayer({ frameX,frameY });
		}

		else
		{
			CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, frameX, frameY);
		}

		if (upgradeReady)
		{
			EFFECT->AfterimageEft("playerFrame", { (long)posX,(long)posY }, { frameX ,frameY }, 5);
		}
	}

	void player::idleDelayCount()
	{
		//��Ż ������ �̹��� ���� ������
		if (idleDelay > 0)
		{
			idleDelay--;
			move = MOVE::DOWN;
			state = STATE::IDLE;
		}

	}

	void player::tileCol()
	{
		for (int i = 0; i < 8; i++)
		{
			tileCheck[i].isCol = false;
			if (i < 4) diagonalCheck[i].isCol = false;
		}

		for (int i = 0; i < vWall.size(); i++)
		{
			int num = vWall[i];

			if (tile[num].keyName != "" && tile[num].kind != TERRAIN::WALL) continue;
			for (int j = 0; j < 8; j++)
			{
				if (colCheck(tileCheck[j].rc, tile[num].rc))
				{
					tileCheck[j].isCol = true;
				}

				if (j < 4 && colCheck(diagonalCheck[j].rc, tile[num].rc) && state == STATE::DASH)
				{
					diagonalCheck[j].isCol = true;
				}
			}
		}
	}

	void player::colorCheck(image * img)
	{
		for (int i = 0; i < 8; i++)
		{
			tileCheck[i].isCol = false;
			if (i < 4)diagonalCheck[i].isCol = false;
		}

		for (int i = 0; i < 8; i++)
		{
			int x = tileCheck[i].rc.left + (tileCheck[i].rc.right - tileCheck[i].rc.left) / 2;
			int y = tileCheck[i].rc.top + (tileCheck[i].rc.bottom - tileCheck[i].rc.top) / 2;

			COLORREF color = GetPixel(img->getMemDC(), x, y);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			if (r == 255 && g == 0 && b == 255)
			{
				tileCheck[i].isCol = true;
			}

			if (i < 4 && state == STATE::DASH)
			{
				x = diagonalCheck[i].rc.left + (diagonalCheck[i].rc.right - diagonalCheck[i].rc.left) / 2;
				y = diagonalCheck[i].rc.top + (diagonalCheck[i].rc.bottom - diagonalCheck[i].rc.top) / 2;

				color = GetPixel(img->getMemDC(), x, y);

				r = GetRValue(color);
				g = GetGValue(color);
				b = GetBValue(color);

				if (r == 255 && g == 0 && b == 255) diagonalCheck[i].isCol = true;
			}
		}
	}

	void player::makeCol(int index, int destX, int destY, int rcSize)
	{
		tileCheck[index].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2) + destX, CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2) + destY, rcSize, rcSize);
	}

	void player::makeCol2(int index, int destX, int destY, int rcSize)
	{
		diagonalCheck[index].rc = RectMakeCenter(CAMERAMANAGER->GetAbsoluteX(WINSIZEX / 2) + destX, CAMERAMANAGER->GetAbsoluteY(WINSIZEY / 2) + destY, rcSize, rcSize);
	}

	void player::resetKey()
	{
		dashLeft = false;
		dashRight = false;
		dashUp = false;
		dashDown = false;
	}

	void player::changeState()
	{
		if (speed > 0)return;
		else
		{
			if (isLeft)
			{
				state = STATE::RUN;
				if (isUp)
				{
					move = MOVE::LEFT_TOP;
				}
				else if (isDown)
				{
					move = MOVE::LEFT_DOWN;
				}
				else // �׳� ���� LEFT
				{
					move = MOVE::LEFT;
				}
			}

			else if (isRight)
			{
				state = STATE::RUN;
				if (isUp)
				{
					move = MOVE::RIGHT_TOP;
				}
				else if (isDown)
				{
					move = MOVE::RIGHT_DOWN;
				}
				else // �׳� ���� LEFT
				{
					move = MOVE::RIGHT;
				}
			}

			else if (isUp) // �׳� ���� UP
			{
				state = STATE::RUN;
				move = MOVE::UP;
			}

			else if (isDown) // �׳� ���� DOWN
			{
				state = STATE::RUN;
				move = MOVE::DOWN;
			}
			else
			{
				state = STATE::IDLE;
			}
		}

	}


	void player::death()
	{
		if (PLAYERDATA->getHp() <= 0)
		{
			isDead = true;
			state = STATE::DIE;
		}
	}
	//�÷��̾� �ǰ� ������
	void player::damage(int damage, float attackAngle, float knockBackSpeed)
	{
		if (PLAYERDATA->getHp() <= 0) return;

		isDamaged = true;

		knockBack.angle = attackAngle;
		knockBack.speed = knockBackSpeed;
		knockBack.percent = 1.0f;

		PLAYERDATA->setHp(PLAYERDATA->getHp() - damage);

		float angle = attackAngle * (180 / PI);

		bool checkAngle = angle > 90 && angle < 270;

		DAMAGE->generator({ (long)posX, (long)posY }, "rNumbers", damage, checkAngle);
		SOUNDMANAGER->play("playerHit", false, -0.18f);

		if (inferno->getGauging() && signatureStateCool != 0 && basic)return;

	}

	void player::damagedCool()
	{
		if (isDamaged)
		{
			frozenTime++;

			if (state == STATE::DASH || state == STATE::SIGNATURE || state == STATE::STANDARD) return;
			state = STATE::DAMAGED;

		}
		if (frozenTime > 20)
		{
			isDamaged = false;
			frozenTime = 0;
			knockBack.angle = 0;
			knockBack.speed = 1.5f;
			knockBack.percent = 1.0f;
		}
	}

	void player::finalAttackDamaged(int damage, int frozenCount)
	{
		if (PLAYERDATA->getHp() <= 0) return;

		isGrabbed = true;
		grabbedTime = frozenCount;

		state = STATE::DAMAGED;

		PLAYERDATA->setHp(PLAYERDATA->getHp() - damage);

		DAMAGE->generator({ (long)posX, (long)posY }, "rNumbers", damage, false);
	}

	void player::grabbedCool()
	{
		if (grabbedTime > 0)
		{
			state = STATE::DAMAGED;
			grabbedTime--;
			if (grabbedTime <= 0)
			{
				grabbedTime = 0;
				isGrabbed = false;
			}
		}
	}

	void player::chargeSkillGauge(int atkPower, int skillNum)
	{
		//basic ������ �� gauge charge�� ����
		if (!upgradeReady && skillGauge <= 100)
		{
			switch (skillNum)
			{
			case 0:
				skillGauge += (float)(atkPower / atkPower) * 1.7f;
				break;
			case 1:
				if (count % 15 == 0)
					skillGauge += (float)(atkPower / atkPower) * 1.2f;
				break;
			case 2:
				if (count % 15 == 0)
					skillGauge += (float)(atkPower / atkPower) * 1.2f;
				break;
			case 3:
				if (count % 20 == 0)
					skillGauge += (float)(atkPower / atkPower) * 1.2f;
				break;
			case 4:
				if (count % 30 == 0)
					skillGauge += (float)(atkPower / atkPower) * 0.7f;
				break;

			case 5:
				if (count % 15 == 0)
					skillGauge += (float)(atkPower / atkPower) * 1.2f;
				break;

			}

		}

	}

	void player::skillGaugeSetUp()
	{

		if (upgradeReady && count % 10 == 0 && skillGauge > 0)
			skillGauge -= 1.f;
		else if (skillGauge > 0 && count % 10 == 0)
			skillGauge -= 0.5f;


		if (skillGauge >= 100)
		{
			skillGauge = 100;
			upgradeReady = true;
		}

		if (upgradeReady && skillGauge <= 0)
			upgradeReady = false;

		PLAYERDATA->setSkillGauge(skillGauge);
		PLAYERDATA->setUpgradeReady(upgradeReady);
	}

	void player::buttonDown()
	{
		//key
		if (INPUT->GetKey(VK_LEFT) || INPUT->GetKey('A'))isLeft = true;
		else isLeft = false;
		if (INPUT->GetKey(VK_RIGHT) || INPUT->GetKey('D'))isRight = true;
		else isRight = false;
		if (INPUT->GetKey(VK_UP) || INPUT->GetKey('W'))isUp = true;
		else isUp = false;
		if (INPUT->GetKey(VK_DOWN) || INPUT->GetKey('S'))isDown = true;
		else isDown = false;
	}
	//del
	void player::viewText()
	{
		//char test[256];
		//wsprintf(test, "basic : %d", basic);
		//textOut(getMemDC(), 100, 170, test, WHITE);
		//wsprintf(test, "standard : %d", standard);
		//textOut(getMemDC(), 100, 190, test, WHITE);
		//wsprintf(test, "signature : %d", signature);
		//textOut(getMemDC(), 100, 210, test, WHITE);

	}