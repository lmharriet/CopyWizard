#include "stdafx.h"
#include "monster.h"



//=============================================
//���̽�Ÿ ���� �Լ�
//=============================================
HRESULT astarManager::init(tagTile* _tile)
{
	tile = _tile;
	//��� �ʱ�ȭ
	startNode = NULL;
	endNode = NULL;
	curNode = NULL;

	//��üŸ�� ��� �ʱ�ȭ
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{
			//���ο� ���� ��Ʈ��ġ ����
			totalNode[x][y] = new tileNode(x, y);
			totalNode[x][y] = &tile[y * MAXTILE_WIDTH + x];
			totalNode[x][y]->idx = x;
			totalNode[x][y]->idy = y;
		}
	}

	//�� ã�ҳ�?
	isFind = false;
	isEnd = isStart = false;
	

	//���� �ʱ�ȭ (����ŸƮ��)
	openList.clear();
	closeList.clear();
	finalList.clear();


	return S_OK;
}

void astarManager::release()
{
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{
			//���ο� ���� ��Ʈ��ġ ����
			SAFE_DELETE(totalNode[x][y]);
			
		}
	}
}

void astarManager::update(RECT _camRC, RECT _monsterRC, RECT _playerRC, float* angle)
{
	//���� ��������
	cam = _camRC;
	monsterPosX = _monsterRC.left + (_monsterRC.right - _monsterRC.left) / 2;
	monsterPosY = _monsterRC.bottom;
	monsterMove.rc = RectMakeCenter(monsterPosX, monsterPosY, 32, 32);
	playerMove.rc = RectMake(_playerRC.left, _playerRC.top, 32, 32);



	//�ʱ�ȭ
	openList.clear();
	closeList.clear();
	finalList.clear();
	startNode = NULL;
	endNode = NULL;
	curNode = NULL;
	isEnd = isStart = false;


	//��ŸƮ�� �ص� ��� ���ϴ� �뵵.
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{
			if (endNode && startNode) break;

			if (colCheck(totalNode[x][y]->rc, playerMove.rc)) // player��ġ ����
			{
				if (totalNode[x][y]->kind == TERRAIN::WALL) continue;
				if (totalNode[x][y]->kind == TERRAIN::NONE) continue;


				totalNode[x][y]->nodeState = NODESTATE::NODE_END;
				endNode = totalNode[x][y];
				playerMove.x = x;
				playerMove.y = y;

				if (isStart)
					break;
				else
					isEnd = true;

			}
			if (colCheck(totalNode[x][y]->rc, monsterMove.rc)) // monster��ġ ����
			{
				if (totalNode[x][y]->kind == TERRAIN::WALL) continue;
				if (totalNode[x][y]->kind == TERRAIN::NONE) continue;

				totalNode[x][y]->nodeState = NODESTATE::NODE_START;
				startNode = totalNode[x][y];
				monsterMove.x = x;
				monsterMove.y = y;

				if (isEnd)
					break;
				else
					isStart = true;

			}
		}
			if (isEnd && isStart)
				break;
	}

	this->pathFinding(); // ��ã�� ����.

	// ���Ϳ��� ���� �ѱ�
	if (!finalList.empty())
	{
		*angle = getAngle(startNode->rc.left, startNode->rc.top, finalList[0]->rc.left, finalList[0]->rc.top);
	}
}


void astarManager::render(HDC hdc)
{
	char str[128];
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{


			//  ���緺Ʈ�� �����ֱ�
			if (colCheck(cam, totalNode[x][y]->rc) == false || totalNode[x][y]->keyName == "") continue;

			if (totalNode[x][y]->kind == TERRAIN::WALL) setNodeColor(totalNode[x][y], RGB(250, 150, 0), hdc);
			//else FrameRect(hdc, totalNode[x][y]->rc, RGB(0, 0, 0));


			//���۳�� �����ֱ�
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_START)
			{
				setNodeColor(startNode, RGB(255, 0, 0), hdc);
				textOut(hdc, startNode->rc.left, startNode->rc.top + 10, "[Start]", RGB(0, 0, 0));
			}
			//������ �����ֱ�
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_END)
			{
				setNodeColor(endNode, RGB(0, 0, 255), hdc);
				textOut(hdc, endNode->rc.left, endNode->rc.top + 20, "[END]");
			}
			//����� �����ֱ�
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_WALL)
			{
				setNodeColor(totalNode[x][y], RGB(200, 150, 100), hdc);
				textOut(hdc, totalNode[x][y]->rc.left + 100, totalNode[x][y]->rc.top + 10, "[WALL]");
			}

		}
	}

	//��ã������ �����ֱ�
	if (isFind)
	{
		for (int i = 0; i < finalList.size(); i++)
		{
			if (i == 0)
			{
				setNodeColor(finalList[i], RGB(255, 255, 0), hdc);
			}
			else
			{
				setNodeColor(finalList[i], RGB(255, 255, 255), hdc);
			}
		}
	}

	FrameRect(hdc, playerMove.rc, RGB(0, 0, 255));
	FrameRect(hdc, monsterMove.rc, RGB(0, 0, 255));
}

void astarManager::pathFinding()
{
	if (!endNode)  return;


	openList.push_back(startNode);

	while (openList.size() > 0)
	{
		curNode = openList[0];


		for (int i = 1; i < openList.size(); i++)
		{
			if (openList[i]->F <= curNode->F && openList[i]->H < curNode->H)
			{
				curNode = openList[i];
			}
		}

		//Ŭ���� ����Ʈ�� �־�д�
		for (int i = 0; i < openList.size(); i++)
		{
			if (curNode == openList[i])
			{
				this->delOpenList(i);
				closeList.push_back(curNode);
			}
		}

		//�����尡 ������ ���� ����? (��ã�Ҵ�)
		if (curNode == endNode)
		{
			tileNode* _endNode = endNode;
			vector<tileNode*> tempNode;
			//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
			while (_endNode != startNode)
			{
				tempNode.push_back(_endNode);
				_endNode = _endNode->parentNode;
			}

			for (int i = tempNode.size() - 1; i > 0; i--)
			{
				finalList.push_back(tempNode[i]);
			}

			isFind = true;
			//�����ϰ� ���� ���´�
			return;
		}

		if(curNode != nullptr)
		{
			addOpenList(curNode->idx + 1, curNode->idy + 1);	//����
			addOpenList(curNode->idx - 1, curNode->idy + 1);	//����
			addOpenList(curNode->idx - 1, curNode->idy - 1);	//�»�
			addOpenList(curNode->idx + 1, curNode->idy - 1);	//���
			addOpenList(curNode->idx, curNode->idy - 1);		//��
			addOpenList(curNode->idx, curNode->idy + 1);		//��
			addOpenList(curNode->idx - 1, curNode->idy);		//��
			addOpenList(curNode->idx + 1, curNode->idy);		//��
		}
	}

}

void astarManager::addOpenList(int idx, int idy)
{
	//����ó�� �ε��� �����ȿ��� �߰��� �� �־�� �Ѵ�

	if (idx < monsterMove.x - 30 || idx >= monsterMove.x + 30 || idy < monsterMove.y - 20 || idy >= monsterMove.y + 20) return;

	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL && totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::NONE && totalNode[idx][curNode->idy]->kind == TERRAIN::NONE) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL || totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::NONE || totalNode[idx][curNode->idy]->kind == TERRAIN::NONE) return;


	//���� ���¸���Ʈ�� ���� �� ����
	if (totalNode[idx][idy]->kind == TERRAIN::WALL) return;

	//Ŭ�����Ʈ(�����±�)�� �ִٸ� ���¸���Ʈ�� ������ �ȵȴ�
	for (int i = 0; i < closeList.size(); i++)
	{
		if (totalNode[idx][idy] == closeList[i]) return;
	}
	//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
	//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��Ѵ�
	tileNode* neighborNode = totalNode[idx][idy];
	int moveCost = curNode->G + ((curNode->idx - idx == 0 || curNode->idy - idy == 0) ? 10 : 14);

	//���¸���Ʈ�ȿ� �̿���尡 ������ �ȵȴ�
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i] == neighborNode) return;
	}


	neighborNode->G = moveCost;
	neighborNode->H = (abs(neighborNode->idx - endNode->idx) + abs(neighborNode->idy - endNode->idy)) * 10;
	neighborNode->F = neighborNode->G + neighborNode->H;
	neighborNode->parentNode = curNode;
	openList.push_back(neighborNode);
}

void astarManager::delOpenList(int index)
{
	openList.erase(openList.begin() + index);
}

//���Ǹ� ���� �Լ�
void astarManager::setNodeColor(tileNode* node, COLORREF color, HDC hdc)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(hdc, &node->rc, brush);
	DeleteObject(brush);
	FrameRect(hdc, node->rc, RGB(0, 0, 0));
}





//=============================================
//���� ���� �Լ�
//=============================================

HRESULT monster::init(tagTile* tile, POINT _pos)
{
	addInit();

	if (kind == MONSTERKIND::SUMMONER)
	{
		isAstar = false;
	}
	else
	{
		isAstar = true;
		astar = make_unique< astarManager>();
		astar->init(tile);
	}

	wall = tile;
	cul = { 0,0 };
	pos.x = _pos.x;
	pos.y = _pos.y;
	hitTime = 0;

	for (int i = 0; i < STATEMAX; i++)
	{
		frameIndexL[i] = { 0,0 };
		frameIndexR[i] = { 0,0 };
	}
	for (int i = 0; i < MONSTER_MAX; i++)
	{
		atkDirection[i] = false;
		bulletDirection[i] = false;
	}




	return S_OK;
}

void monster::release()
{
	if (isAstar)
	{
		astar->release();
	}
}

void monster::commonUpdate()
{

	rc = RectMake(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());

	if (distanceMax > getDistance(pos.x + img->getFrameWidth() * 0.5, pos.y + img->getFrameHeight() * 0.5, playerRC.left, playerRC.top))
	{
		isFindWayOn = true;
		isCardAppear = true;
	}
	else
		isFindWayOn = false;

	if (isCardAppear)
	{
		if (!isCardFxAppear)
		{
			if (kind == MONSTERKIND::GOLEM && kind == MONSTERKIND::GHOULLARGE)
				EFFECT->setEffect("flipCard_big", { pos.x + img->getFrameWidth() / 2,pos.y + img->getFrameHeight() / 2 }, false, true, 100);
			else if (kind != MONSTERKIND::KNIGHT)
				EFFECT->setEffect("flipCard_small", { pos.x + img->getFrameWidth() / 2,pos.y + img->getFrameHeight() / 2 }, false, true, 100); // false,false,0,
			else
				EFFECT->setEffect("flipCard_small", { pos.x + img->getFrameWidth() / 2,pos.y + img->getFrameHeight() / 2 + 70 }, false, true, 100); // false,false,0,
			isCardFxAppear = true;
		}
		monsterAppearCount++;
		if (monsterAppearCount == 50)
		{
			isMonsterApeear = true;
		}
	}
	if (isMonsterApeear)
	{
		if (isIceState)
			iceBreakCount++;
		
		update(); //���ͺ� ������Ʈ

		
		if (isHit && !isDie)
		{
			if (kind != MONSTERKIND::GOLEM && kind!=MONSTERKIND::GHOULLARGE)
				isATK = false;
			hitTime++;

			if (isKnockBack) // �з���.
			{
				
				if (!wallCol())
				{
					pos.x += cos(hitAngle) * knockBack * 0.25;
					pos.y += -sin(hitAngle) * knockBack * 0.25;
				}
			}

			if (hitTime % 6 == 0)
			{
				isHit = false;
				hitTime = 0;
			}
		}

		hitCul();

		cul.x = CAMERAMANAGER->GetRelativeX(pos.x);
		cul.y = CAMERAMANAGER->GetRelativeY(pos.y);

		if (!isDie)
			die();
		else
		{
			state = STATEIMAGE::DIE;
		}
		stateImageRender();

		if (iceBreakCount > iceBreakTime)
		{
			isIceState = false;
			iceBreakCount = 0;
			//EFFECT->setEffect("iceBreak")
		}
	}
}

void monster::render()
{

	addRender();
	if (isIceState)
		CAMERAMANAGER->AlphaRender(getMemDC(), IMAGEMANAGER->findImage("IceState"), pos.x, getCenterY()-img->getFrameHeight(),180);
	// astar->render(getMemDC());
	 //FrameRect(getMemDC(), playerRC, RGB(255, 255, 255));
	 //FrameRect(getMemDC(), rc, RGB(255, 255, 255));
}

void monster::addRender()
{
}

void monster::setPt(float x, float y)
{
	if (!wallCol())
		pos = { (long)x, (long)y };
}

void monster::hit(int damage, float _hitAngle, float _knockBack, int skillNum, bool isCritical, bool isIceSTATE)
{
	if (hitCheck(skillNum) == false)return;

	POINT pt = { pos.x + img->getFrameWidth() / 2,pos.y + img->getFrameHeight() / 2 };

	EFFECT->damageEffect(pt);

	int allDamage = (int)((float)damage * armour);
	if (allDamage > 0)
		hp -= allDamage;

	if (cos(_hitAngle) * 2.f > 0)
		DAMAGE->generator(pt, "numbers", allDamage, false, isCritical);
	else
		DAMAGE->generator(pt, "numbers", allDamage, true, isCritical);

	hitAngle = _hitAngle;
	knockBack = _knockBack;

	isHit = true;

	char knight[50];
	char ghoul[50];
	sprintf(knight, "knightHit%d", RANDOM->range(2));
	sprintf(ghoul, "ghoulHit%d", RANDOM->range(2));
	//hit sound
	switch (kind)
	{
	case MONSTERKIND::GOLEM:
		SOUNDMANAGER->play("golemHit", false, -0.28f);
		break;
	case MONSTERKIND::KNIGHT:
		SOUNDMANAGER->play(knight, false, -0.28f);
		state = STATEIMAGE::HIT;
		isIceState = isIceSTATE;
		break;
	case MONSTERKIND::SUMMONER:
		SOUNDMANAGER->play("golemHit", false, -0.28f);
		state = STATEIMAGE::HIT;
		isIceState = isIceSTATE;
		break;
	case MONSTERKIND::GHOUL:
		SOUNDMANAGER->play(ghoul, false,-0.28f );
		state = STATEIMAGE::HIT;
		isIceState = isIceSTATE;
	case MONSTERKIND::GHOULLARGE:
		SOUNDMANAGER->play(ghoul, false, -0.28f);
		isIceState = isIceSTATE;
		break;

	}
}

bool monster::hitCheck(int skillNum)
{
	for (int i = 0; i < vHit.size(); i++)
	{
		if (vHit[i].skillNum == skillNum)return false;
	}
	tagHit1 hit;
	hit.skillNum = skillNum;
	hit.currentTime = 0;
	switch (hit.skillNum)
	{
	case 0:
		hit.endTime = 2;
		break;
	case 1:
		hit.endTime = 10;
		break;
	case 2:
		hit.endTime = 15;
		break;
	case 3:
		hit.endTime = 20;
		break;
	case 4:
		hit.endTime = 7;
		break;
	case 5:
		hit.endTime = 15;
		break;
	}
	vHit.push_back(hit);
	
	return true;
}

void monster::hitCul()
{
	for (int i = 0; i < vHit.size(); )
	{
		//���� Ȯ��
		if (vHit[i].currentTime == vHit[i].endTime)
		{
			vHit.erase(vHit.begin() + i);
		}
		else
		{
			vHit[i].currentTime++;
			i++;
		}

	}
}

bool monster::wallCol()
{
	for (int i = 0; i < PLAYERDATA->getWall().size(); i++)
	{
		if (kind == MONSTERKIND::GHOULLARGE)
		{
			RECT ghoulDashRC = RectMakeCenter(this->getCenterX(), this->getCenterY(), 50, 50);
			if (colCheck(ghoulDashRC, wall[PLAYERDATA->getWall()[i]].rc))
			{
				return true;
			}
		}
		else
		{
			if (colCheck(rc, wall[PLAYERDATA->getWall()[i]].rc))
			{
				return true;
			}
		}
	}
	return false;
}

void monster::coinDrop(int min, int max)
{
	if (isDelete)
	{
		DROP->dropPoint({ pos.x + img->getFrameWidth() / 2,pos.y + img->getFrameHeight() }, min, max);
	}
}

void monster::die()
{
	if (hp <= 0)
	{

		isDie = true;

		switch (kind)
		{
		case MONSTERKIND::GOLEM:
			SOUNDMANAGER->play("golemDie", false, -0.2f);
			break;
		case MONSTERKIND::KNIGHT:
			SOUNDMANAGER->play("knightDie", false, -0.3f);
			break;
		case MONSTERKIND::SUMMONER:
			SOUNDMANAGER->play("knightDie", false, -0.3f);
			break;
		case MONSTERKIND::GHOUL:
			SOUNDMANAGER->play("ghoulDie", false); 
			break;
		case MONSTERKIND::GHOULLARGE:
			SOUNDMANAGER->play("golemDie", false, -0.2f);
			break;

		}

	}
}

void monster::stateHIT(POINT lPos, POINT rPos)
{
	if (atkDirection[MONSTER_LEFT])
	{
		frameIndexL[STATEIMAGE::HIT].x = lPos.x;
		frameIndexL[STATEIMAGE::HIT].y = lPos.y;
		currentFrame = { frameIndexL[STATEIMAGE::HIT].x ,frameIndexL[STATEIMAGE::HIT].y };
		//img->frameRender(getMemDC(), cul.x, cul.y, frameIndexL[STATEIMAGE::HIT].x, frameIndexL[STATEIMAGE::HIT].y);
	}
	else
	{
		frameIndexR[STATEIMAGE::HIT].x = rPos.x;
		frameIndexR[STATEIMAGE::HIT].y = rPos.y;
		currentFrame = { frameIndexR[STATEIMAGE::HIT].x ,frameIndexR[STATEIMAGE::HIT].y };
		//img->frameRender(getMemDC(), cul.x, cul.y, frameIndexR[STATEIMAGE::HIT].x, frameIndexR[STATEIMAGE::HIT].y);
	}
}
