#include "stdafx.h"
#include "monster.h"



//=============================================
//에이스타 관련 함수
//=============================================
HRESULT astarManager::init(tagTile* _tile)
{
	tile = _tile;
	//노드 초기화
	startNode = NULL;
	endNode = NULL;
	curNode = NULL;

	//전체타일 노드 초기화
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{
			//새로운 노드와 렉트위치 설정
			totalNode[x][y] = new tileNode(x, y);
			totalNode[x][y] = &tile[y * MAXTILE_WIDTH + x];
			totalNode[x][y]->idx = x;
			totalNode[x][y]->idy = y;
		}
	}

	//길 찾았냐?
	isFind = false;

	//벡터 초기화 (리스타트용)
	openList.clear();
	closeList.clear();
	finalList.clear();


	return S_OK;
}

void astarManager::release()
{
}

void astarManager::update(RECT _camRC, RECT _monsterRC, RECT _playerRC, float* angle)
{
	//정보 가져오기
	cam = _camRC;
	monsterPosX =_monsterRC.left + (_monsterRC.right - _monsterRC.left) / 2;
	monsterPosY = _monsterRC.bottom;
	monsterMove.rc = RectMakeCenter(monsterPosX,monsterPosY, 32,32);
	playerMove.rc = RectMake(_playerRC.left, _playerRC.top, 32, 32);



	//초기화
	openList.clear();
	closeList.clear();
	finalList.clear();
	startNode = NULL;
	endNode = NULL;
	curNode = NULL;
	

	//스타트랑 앤드 노드 정하는 용도.
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{
			if (endNode && startNode) break;

			if (colCheck(totalNode[x][y]->rc, playerMove.rc)) // player위치 설정
			{
				if (totalNode[x][y]->kind == TERRAIN::WALL) continue;
				if (totalNode[x][y]->kind == TERRAIN::NONE) continue;
				

				totalNode[x][y]->nodeState = NODESTATE::NODE_END;
				endNode = totalNode[x][y];
				playerMove.x = x;
				playerMove.y = y;

			}
			if (colCheck(totalNode[x][y]->rc, monsterMove.rc)) // monster위치 설정
			{
				if (totalNode[x][y]->kind == TERRAIN::WALL) continue;
				if (totalNode[x][y]->kind == TERRAIN::NONE) continue;
				
				totalNode[x][y]->nodeState = NODESTATE::NODE_START;
				startNode = totalNode[x][y];
				monsterMove.x = x;
				monsterMove.y = y;
			}


		}

	}
	
	//벽(장애물) 노드 세팅하기 (시작, 종료노드 설정전에 벽세우지 못하게 막기)
	//if (INPUT->GetKeyDown(VK_RBUTTON) && startNode && endNode)
	//{
	//	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	//	{
	//		for (int x = 0; x < MAXTILE_WIDTH; x++)
	//		{
	//			if (PtInRect(&totalNode[x][y]->rc, _ptMouse))
	//			{
	//				//시작노드, 종료노드는 선택하지 못하게 막기
	//				if (totalNode[x][y]->nodeState == NODESTATE::NODE_START) continue;
	//				if (totalNode[x][y]->nodeState == NODESTATE::NODE_END) continue;
	//				totalNode[x][y]->nodeState = NODESTATE::NODE_WALL;
	//			}
	//		}
	//	}
	//}
	

	
	this->pathFinding(); // 길찾기 시작.

	// 몬스터에게 각도 넘김
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
		

			//  현재렉트만 보여주기
			if (colCheck(cam, totalNode[x][y]->rc) == false || totalNode[x][y]->keyName == "") continue;
			
			if (totalNode[x][y]->kind == TERRAIN::WALL) setNodeColor(totalNode[x][y], RGB(250, 150, 0), hdc);
			//else FrameRect(hdc, totalNode[x][y]->rc, RGB(0, 0, 0));


			//시작노드 보여주기
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_START)
			{
				setNodeColor(startNode, RGB(255, 0, 0), hdc);
				textOut(hdc, startNode->rc.left, startNode->rc.top + 10, "[Start]", RGB(0, 0, 0));
			}
			//종료노드 보여주기
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_END)
			{
				setNodeColor(endNode, RGB(0, 0, 255), hdc);
				textOut(hdc, endNode->rc.left, endNode->rc.top + 20, "[END]");
			}
			//벽노드 보여주기
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_WALL)
			{
				setNodeColor(totalNode[x][y], RGB(200, 150, 100), hdc);
				textOut(hdc, totalNode[x][y]->rc.left + 100, totalNode[x][y]->rc.top + 10, "[WALL]");
			}
		
		}
	}

	//길찾았을때 보여주기
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

		//클로즈 리스트에 넣어둔다
		for (int i = 0; i < openList.size(); i++)
		{
			if (curNode == openList[i])
			{

				this->delOpenList(i);
				closeList.push_back(curNode);
			}
		}

		//현재노드가 마지막 노드와 같냐? (길찾았다)
		if (curNode == endNode)
		{
			

			tileNode* _endNode = endNode;
			vector<tileNode*> tempNode;
			//마지막 노드로부터 시작노드까지 부모노드를 벡터에 담는다
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
			//종료하고 빠져 나온다
			return;
		}

		
	 	addOpenList(curNode->idx + 1, curNode->idy + 1);	//우하
		addOpenList(curNode->idx - 1, curNode->idy + 1);	//좌하
		addOpenList(curNode->idx - 1, curNode->idy - 1);	//좌상
		addOpenList(curNode->idx + 1, curNode->idy - 1);	//우상
		addOpenList(curNode->idx, curNode->idy - 1);		//상
		addOpenList(curNode->idx, curNode->idy + 1);		//하
		addOpenList(curNode->idx - 1, curNode->idy);		//좌
		addOpenList(curNode->idx + 1, curNode->idy);		//우
	}

}

void astarManager::addOpenList(int idx, int idy)
{
	//예외처리 인덱스 범위안에서 추가할 수 있어야 한다
	
	if (idx < monsterMove.x - 30 || idx >= monsterMove.x + 30 || idy < monsterMove.y - 20 || idy >= monsterMove.y + 20) return;

	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL && totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::NONE && totalNode[idx][curNode->idy]->kind == TERRAIN::NONE) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL || totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::NONE || totalNode[idx][curNode->idy]->kind == TERRAIN::NONE) return;


	//벽은 오픈리스트에 담을 수 없다
	if (totalNode[idx][idy]->kind == TERRAIN::WALL) return;

	//클로즈리스트(지나온길)에 있다면 오픈리스트에 담으면 안된다
	for (int i = 0; i < closeList.size(); i++)
	{
		if (totalNode[idx][idy] == closeList[i]) return;
	}
	//여기까지 왔으면 문제가 없으니 이제 오픈리스트에 추가를 하자
	//현재노드의 4방향 노드를 이웃노드라고 하고 직선10, 대각은 14비용을 추가한다
	tileNode* neighborNode = totalNode[idx][idy];
	int moveCost = curNode->G + ((curNode->idx - idx == 0 || curNode->idy - idy == 0) ? 10 : 14);

	//오픈리스트안에 이웃노드가 있으면 안된다
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

//편의를 위한 함수
void astarManager::setNodeColor(tileNode* node, COLORREF color, HDC hdc)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(hdc, &node->rc, brush);
	DeleteObject(brush);
	FrameRect(hdc, node->rc, RGB(0, 0, 0));
}





//=============================================
//몬스터 관련 함수
//=============================================

HRESULT monster::init(tagTile* tile, POINT _pos)
{
	addInit();

	if (kind==MONSTERKIND::SUMMONER)
	{
		isAstar = false;
	}
	else
	{
		isAstar = true;
		astar = new astarManager;
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
		SAFE_DELETE(astar);
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
			if(kind != MONSTERKIND::KNIGHT)
			EFFECT->setEffect("flipCard", { pos.x + img->getFrameWidth()/2,pos.y + img->getFrameHeight()/2 },false,true,100); // false,false,0,
			else
			EFFECT->setEffect("flipCard", { pos.x + img->getFrameWidth()/2,pos.y + img->getFrameHeight()/2+70 },false,true,100); // false,false,0,
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
		update(); //몬스터별 업데이트

		if (isHit && !isDie)
		{
			if (kind != MONSTERKIND::GOLEM)
				isATK = false;
			hitTime++;

			if (isKnockBack) // 밀려남.
			{
				wallCol();
				if (!isWallcol)
				{
					pos.x += cos(hitAngle) * knockBack * 0.25;
					pos.y += -sin(hitAngle) * knockBack * 0.25;
				}
				isWallcol = false;
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
	}
	if (isMonsterApeear)
		stateImageRender();
}

void monster::render()
{
	
	// astar->render(getMemDC());
	 //FrameRect(getMemDC(), playerRC, RGB(255, 255, 255));
	 //FrameRect(getMemDC(), rc, RGB(255, 255, 255));
}

void monster::setPt(float x, float y)
{
	wallCol();
	if(!isWallcol)
	 pos = { (long)x, (long)y }; 

	isWallcol = false;
}

void monster::hit(int damage , float _hitAngle, float _knockBack, int skillNum, bool isCritical)
{
	if (hitCheck(skillNum) == false)return;

	POINT pt = { pos.x + img->getFrameWidth() / 2,pos.y + img->getFrameHeight() / 2 };

	EFFECT->damageEffect(pt);

	int allDamage = damage - armour;
	if(allDamage>0)
		hp -= allDamage;

	if(cos(_hitAngle)*2.f>0)
		DAMAGE->generator(pt, "numbers", allDamage,false, isCritical);
	else
		DAMAGE->generator(pt, "numbers", allDamage,true, isCritical);

	hitAngle = _hitAngle;
	knockBack = _knockBack;
	
	
	if(kind != MONSTERKIND::GOLEM)
		state = STATEIMAGE::HIT;

	isHit = true;
	
	//hit sound
	switch (kind)
	{
	case MONSTERKIND::GOLEM:
		SOUNDMANAGER->play("golemHit", false,-0.28f);
		break;
	case MONSTERKIND::KNIGHT:
		char str[50];
		sprintf(str, "knightHit%d", RANDOM->range(2));
		SOUNDMANAGER->play(str, false,-0.38f);
		break;
	case MONSTERKIND::SUMMONER:
		SOUNDMANAGER->play("golemHit", false,-0.3f); 
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
	}
	vHit.push_back(hit);

	return true;
}

void monster::hitCul()
{
	for (int i = 0; i < vHit.size(); )
	{
		//삭제 확인
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

void monster::wallCol()
{
	for (int i = 0; i < PLAYERDATA->getWall().size(); i++)
	{
		if (colCheck(rc, wall[PLAYERDATA->getWall()[i]].rc))
		{
			isWallcol = true;
			return;
		}
	}
}

void monster::coinDrop(int min, int max)
{
	if (isDelete)
	{
		DROP->dropPoint({ pos.x+img->getFrameWidth()/2,pos.y + img->getFrameHeight() },min,max);
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
			SOUNDMANAGER->play("knightDie", false,-0.3f);
			
			break;
		case MONSTERKIND::SUMMONER:
			SOUNDMANAGER->play("knightDie", false,-0.3f); //hit sound change
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
