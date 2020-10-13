#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerFrame_small1.bmp", 1000, 2500, 10, 25);
	IMAGEMANAGER->addFrameImage("PlayerAttackCircle", "resource/player/PlayerAttackCircle1.bmp", 3600, 100, 36, 1);
	IMAGEMANAGER->addFrameImage("meteor", "resource/player/meteor.bmp", 1200, 250, 6, 1);
	IMAGEMANAGER->addFrameImage("flame", "resource/player/flame1.bmp", 4096, 128, 32, 1);
	IMAGEMANAGER->addFrameImage("flameStrike", "resource/player/flameStrike1.bmp", 1707, 171, 10, 1);
	IMAGEMANAGER->addImage("infernoGauging", "Images/ui/gaugingBar.bmp", 50, 10);

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
	basic = standard = signature = false;

	speed = gaugeTime = 0;

	//animation count ,index
	atkCount = atkIndex = index = dashIndex = count = dashCount = basicCount = basicIndex = 0;
	stateCool = infernoStateCool = meteorStateCool = 0;


	//blaze 3�� �߻� �뵵
	blazeCount = 0;

	//�ҷ� Ŭ����
	bulletClassInit();
	//�κ��丮
	inven = new inventory;
	inven->init();

	//angle between mouse & player
	attackAngle = saveAngle = saveAngle2 = 0;
	angleTenth = 0;

	//damage 
	damageAngle = 0;
	damageAngleTenth = frozenTime = 0;

	isDamaged = false;
	isDead = false;

	//upgrade gauge
	upgradeGauge = 0;
	gaugeMax = false;
	upgrade = false;




	//test
	arcana.name = "nonSkill";
	arcana.type = ARCANA::EMPTY;


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

	inferno->update(&gaugeTime);

	inven->update();
	//animation count
	count++;
	dashCount++;
	atkCount++;
	basicCount++;

	gaugeTime++;
	PLAYERDATA->setGaugeTime(gaugeTime);
	// angle(mouse-player), angleTenth
	attackAngle = getAngle(posX, posY, CAMERAMANAGER->GetAbsoluteX(_ptMouse.x), CAMERAMANAGER->GetAbsoluteY(_ptMouse.y));
	angleTenth = (int)(saveAngle * (18 / PI));

	if (speed == 0 && stateCool == 0 && meteorStateCool == 0 && frozenTime == 0
		&& !isDead && !inferno->getGauging())
	{
		controller();
	}

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


	dashFunction();
	changeState();
	blazeSetUp();
	infernoSetUp();
	meteorSetUp();

	dragonArcSetUp();

	damagedCool();

	// knockBack lerp
	if (isDamaged)
	{
		if (tileCheck[index].isCol || tileCheck[index + 4].isCol || diagonalCheck[index].isCol)return;
		knockBack.percent -= 0.2f;
		posX += cosf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		posY -= sinf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		rc = RectMakeCenter(posX, posY, 50, 50);
	}
	
	takeCoin();
	takeHealball();

	// camera�� ���󰡴� ���
	CAMERAMANAGER->MovePivot(posX, posY);
	death();
	//don't touch!
	buttonDown();



	//gauge
	if (upgradeGauge < 100 && upgradeGauge >= 0 && count % 10 == 0) upgradeGauge -= 0.5f;

	if (upgradeGauge == 100)
		gaugeMax = true;
}

void player::other_update()
{
	PLAYERDATA->update();

	blaze->update();
	dragon->update();
	Meteor->update();
	searingRush->update();

	inferno->update(&gaugeTime);
	gaugeTime++;


	inven->update();

	//animation count
	count++;
	dashCount++;
	atkCount++;
	basicCount++;

	gaugeTime++;
	PLAYERDATA->setGaugeTime(gaugeTime);

	// angle(mouse-player), angleTenth
	attackAngle = getAngle(posX, posY, CAMERAMANAGER->GetAbsoluteX(_ptMouse.x), CAMERAMANAGER->GetAbsoluteY(_ptMouse.y));
	angleTenth = (int)(saveAngle * (18 / PI));


	if (speed == 0 && stateCool == 0 && meteorStateCool == 0 && !isDead && !inferno->getGauging())
	{
		controller();
	}

	//tileCol(); 

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


	dashFunction();
	changeState();
	blazeSetUp();
	infernoSetUp();
	meteorSetUp();

	dragonArcSetUp();

	damagedCool();

	// knockBack lerp
	if (isDamaged)
	{
		if (tileCheck[index].isCol || tileCheck[index + 4].isCol || diagonalCheck[index].isCol)return;
		knockBack.percent -= 0.2f;
		posX += cosf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		posY -= sinf(knockBack.angle) * (knockBack.speed + knockBack.percent);
		rc = RectMakeCenter(posX, posY, 50, 50);
	}
	takeCoin();
	takeHealball();

	// camera�� ���󰡴� ���
	CAMERAMANAGER->MovePivot(posX, posY);
	death();

	//don't touch!
	buttonDown();

}

void player::render()
{
	int tempAngle = attackAngle * (18 / PI);
	image* img = IMAGEMANAGER->findImage("PlayerAttackCircle");
	CAMERAMANAGER->AlphaFrameRender(getMemDC(), img, posX - 50, posY - 20, tempAngle, 0, 50);

	bool isRender = false;

	// DASH FIRE RENDER
	for (int i = 0; i < searingRush->getSize(); i++)
	{
		if (!isRender && (searingRush->getY(i) > posY))
		{
			isRender = true;
			animation();
			searingRush->singleRender(i);
		}
		else searingRush->singleRender(i);
	}

	if (!isRender)animation(); // z���� ���� ������.. (���߿� frame�� �������� �����ϴ� ���..)

	blaze->render();
	dragon->render();
	Meteor->render();
	inferno->render();

	if (inferno->getGauging())
	{
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("infernoGauging"), posX, posY - 70);
	}

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
	blaze->init(3, 350);

	dragon = new dragonArc;
	dragon->init();

	Meteor = new meteor;
	Meteor->init();

	searingRush = new dashFire;
	searingRush->init();

	inferno = new RagingInferno;
	inferno->init();
}

void player::controller()
{
	//����
	if (isLeft)
	{
		rc = RectMakeCenter(posX, posY, 50, 50);
		if (!tileCheck[(int)DIRECTION::LEFT].isCol /*&& !tileCheck[(int)DIRECTION::LEFT_DOWN].isCol && !tileCheck[(int)DIRECTION::LEFT_TOP].isCol*/)
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
		if (!tileCheck[(int)DIRECTION::RIGHT].isCol /*&& !tileCheck[(int)DIRECTION::RIGHT_DOWN].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol*/)
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
		if (/*!tileCheck[(int)DIRECTION::TOP].isCol &&*/ !tileCheck[(int)DIRECTION::LEFT_TOP].isCol && !tileCheck[(int)DIRECTION::RIGHT_TOP].isCol)
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

void player::dashFunction()
{
	// ���� ������ �뽬 �Ұ���, ���� ������ �뽬�ؼ� ���������� ������ ������. 
	// ������ ����ó�� �ʿ�

	if (speed == 0) return;

	if (!searingRush->getIsCoolTime() &&
		(speed == 8 || speed == 11 || speed == 14 || speed == 17))

		searingRush->fire(posX, posY);

	if (!searingRush->getIsCoolTime() && speed == 17) //sound
		SOUNDMANAGER->play("playerfireDash", false);

	if (speed == 7)
	{
		UI->addCoolTime("searingDash");
		searingRush->setIsCoolTime(true);


	}

	if (dashLeft)
	{
		if (dashUp)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_TOP].isCol &&
				!diagonalCheck[0].isCol)
			{
				posX -= speed;
				posY -= speed;
				OffsetRect(&rc, -speed, -speed);
				RectMakeCenter(posX, posY, 50, 50);
			}
		}
		else if (dashDown)
		{
			if (!tileCheck[(int)DIRECTION::LEFT_DOWN].isCol &&
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
			if (!tileCheck[(int)DIRECTION::LEFT].isCol)

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
				!diagonalCheck[1].isCol)
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

void player::blazeSetUp()
{
	if (INPUT->GetKeyDown(VK_LBUTTON) && !blaze->getCool() && frozenTime == 0 && !isDead
		&& !inferno->getGauging() && meteorStateCool == 0 && speed == 0)
	{
		UI->addCoolTime(0);
		blazeCount = 3;
		basic = true;
		saveAngle = attackAngle;
	}

	if (stateCool == 0 && basic)
	{
		//basic ���� �� �� �ޱ��� ����

		stateCool = 10;

		float x = cosf(attackAngle) * 50.f + posX;
		float y = -sinf(attackAngle) * 50.f + posY;

		blaze->fire(x, y, 10, saveAngle, 2);
		blazeCount--;
	}

	if (stateCool > 0)
	{
		state = STATE::BASIC;
		stateCool--;

	}

	if (blazeCount == 0)basic = false;

	for (int i = 0; i < blaze->getSize(); i++)
	{
		if (blaze->getCol(i))
		{
			blaze->removeBomb(i);
		}
	}
	//	//	// ����� �ޱ� �������� �����̱�
	//	//	/*posX += cosf(saveAngle);
	//	//	posY += -sinf(saveAngle);*/
	//	//}
	//	//else basic = false;
}

void player::infernoSetUp()
{
	if (INPUT->GetKeyDown(VK_RBUTTON) && frozenTime == 0 && !isDead
		&& !inferno->getCool() && meteorStateCool == 0 && speed == 0)
	{
		standard = true;
		saveAngle2 = attackAngle;
	}
	else standard = false;

	if (standard)
		inferno->fire(posX, posY, attackAngle, &gaugeTime);

	if (inferno->getGauging())
		state = STATE::STANDARD;

}

void player::meteorSetUp()
{
	float mouseX = CAMERAMANAGER->GetAbsoluteX(_ptMouse.x);
	float mouseY = CAMERAMANAGER->GetAbsoluteY(_ptMouse.y);
	//Attack
	if (!gaugeMax)
	{
		if (INPUT->GetKeyDown('Q') && frozenTime == 0 && !isDead
			&& !Meteor->getCool() && !inferno->getGauging() && speed == 0)
		{
			signature = true;

			if (meteorStateCool == 0)
			{
				meteorStateCool = 30;
				Meteor->creatMeteor(mouseX, mouseY, 0);
			}
		}
		if (meteorStateCool > 0)
		{
			state = STATE::SIGNATURE;
			meteorStateCool--;

			if (meteorStateCool == 0) signature = false;
		}
	}
	else
	{
		if (INPUT->GetKeyDown('Q') && frozenTime == 0 && !isDead
			&& !Meteor->getCool() && !inferno->getGauging() && speed == 0)
		{
			signature = true;

			if (meteorStateCool == 0)
			{
				meteorStateCool = 30;
				Meteor->creatMeteor(mouseX, mouseY, 0);
			}
		}
		if (meteorStateCool > 0)
		{
			state = STATE::SIGNATURE;
			meteorStateCool--;

			if (meteorStateCool == 0) signature = false;
		}
	}
}

void player::dragonArcSetUp()
{
	if (INPUT->GetKeyDown('E'))
	{
		float angle = getAngle(posX, posY, CAMERAMANAGER->GetAbsoluteX(_ptMouse.x), CAMERAMANAGER->GetAbsoluteY(_ptMouse.y));

		//dragon->fire(posX, posY, angle);
		dragon->phoenixFire(posX, posY, angle);

	}
}

void player::takeCoin()
{
	for (int i = 0; i < DROP->getCoinVec().size(); i++)
	{
		if (colCheck(rc, DROP->getCoinRect(i)))
		{
			PLAYERDATA->setCoin(PLAYERDATA->getCoin() + DROP->getCoinVec()[i].money);
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

void player::animation()
{
	int tempAngle = saveAngle2 * (18 / PI);

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
				if (dashIndex > 9) dashIndex = 0;
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
			if (dashCount % 5 == 0)
			{
				dashIndex++;
				if (dashIndex > 9 || speed == 0)dashIndex = 0;
			}
			frameAnimation(dashIndex, 19);
		}
		break;
	case STATE::DAMAGED:
		if (frozenTime > 0)
		{
			frameAnimation(6, 0);
			if (!isDamaged) move = MOVE::LEFT;
		}
		//�������� 

		//if (damageAngleTenth > 14 && damageAngleTenth <= 23)//left
		//{
		//	frameAnimation(6, 0);
		//	if (!isDamaged) move = MOVE::LEFT;
		//}
		//else if (damageAngleTenth <= 4 || damageAngleTenth > 32) //right
		//{
		//	frameAnimation(7, 0);
		//	if (!isDamaged) move = MOVE::RIGHT;
		//}
		//else if (damageAngleTenth > 4 && damageAngleTenth <= 14) //up
		//{
		//	frameAnimation(5, 0);
		//	if (!isDamaged) move = MOVE::UP;
		//}
		//else if (damageAngleTenth > 23 && damageAngleTenth <= 32) //down
		//{
		//	frameAnimation(4, 0);
		//	if (!isDamaged)	move = MOVE::DOWN;
		//}
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
		{
			if (basicCount % 5 == 0)
			{
				basicIndex++;
				if (basicIndex > 6 || stateCool == 0) basicIndex = 0;
			}
			frameAnimation(basicIndex, 5);
			//���� ���� ������ ���� ���ϱ�
			if (stateCool == 0)
				move = MOVE::LEFT;
		}
		else if (angleTenth <= 4 || angleTenth > 32) //right
		{
			if (basicCount % 5 == 0)
			{
				basicIndex++;
				if (basicIndex > 6 || stateCool == 0) basicIndex = 0;
			}
			frameAnimation(basicIndex, 6);

			// ������ ���� ������ ������ ���ϱ�
			if (basicCount == 0)
				move = MOVE::RIGHT;
		}
		else if (angleTenth > 4 && angleTenth <= 14) //up
		{
			if (basicCount % 5 == 0)
			{
				basicIndex++;
				if (basicIndex > 6 || stateCool == 0)basicIndex = 0;
			}
			frameAnimation(basicIndex, 14);
			//���� ���� ������ ���� ���ϱ�
			if (stateCool == 0)
				move = MOVE::UP;
		}
		else if (angleTenth > 23 && angleTenth <= 32) //down
		{
			if (basicCount % 5 == 0)
			{
				basicIndex++;
				if (basicIndex > 6 || stateCool == 0)basicIndex = 0;
			}
			frameAnimation(basicIndex, 6);
			//�Ʒ��� ���� ������ �Ʒ��� ���ϱ�
			if (stateCool == 0)
				move = MOVE::DOWN;
		}
		break;
	case STATE::STANDARD:

		if (tempAngle > 14 && tempAngle <= 23)//left
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
		else if (tempAngle <= 4 || tempAngle > 32) //right
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
		else if (tempAngle > 4 && tempAngle <= 12) //up
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
		else if (tempAngle > 23 && tempAngle <= 32) //down
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
			frameAnimation(atkIndex, 10);
		}
		else if (move == MOVE::RIGHT)
		{
			if (atkCount % 10 == 0)
			{
				atkIndex++;
				if (atkIndex > 9)atkIndex = 0;
			}
			frameAnimation(atkIndex, 20);
		}
		else if (move == MOVE::UP || move == MOVE::LEFT_TOP || move == MOVE::RIGHT_TOP)
		{
			if (atkCount % 10 == 0)
			{
				atkIndex++;
				if (atkIndex > 9)atkIndex = 0;
			}
			frameAnimation(atkIndex, 20);
		}
		else if (move == MOVE::DOWN || move == MOVE::LEFT_DOWN || move == MOVE::RIGHT_DOWN)
		{
			if (atkCount % 10 == 0)
			{
				atkIndex++;
				if (atkIndex > 9)atkIndex = 0;
			}
			frameAnimation(atkIndex, 10);
		}
		break;
	}

}

void player::frameAnimation(int frameX, int frameY)
{
	CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("playerFrame"), posX - 50, posY - 50, frameX, frameY);
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

void player::colorCheck(image* img)
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


	if (inferno->getGauging() && meteorStateCool != 0 && basic)return;

	int index;
	for (int i = 0; i < 4; i++)	index = i;
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

void player::skillGauge(bool appearCri)
{
	bool cri = appearCri;
	if (cri) upgradeGauge += 5.f;
	else upgradeGauge += 2.f;
}
void player::arcanaCheck()
{
}

//del
void player::viewText()
{
	//wsprintf(str, "damageAngle : %d", damageAngle);
	//textOut(getMemDC(), 100, 230, str, WHITE);


	//wsprintf(str, "damageAngleTenth : %d", damageAngleTenth);
	//textOut(getMemDC(), 100, 250, str, WHITE);

}