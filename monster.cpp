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

	//���� �ʱ�ȭ (����ŸƮ��)
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
	//���� ��������
	cam = _camRC;
	monsterPosX =_monsterRC.left + (_monsterRC.right - _monsterRC.left) / 2;
	monsterPosY = _monsterRC.bottom;
	monsterMove.rc = RectMakeCenter(monsterPosX,monsterPosY, 32,32);
	playerMove.rc = RectMake(_playerRC.left, _playerRC.top, 32, 32);



	//�ʱ�ȭ
	openList.clear();
	closeList.clear();
	finalList.clear();
	startNode = NULL;
	endNode = NULL;
	curNode = NULL;
	

	//��ŸƮ�� �ص� ��� ���ϴ� �뵵.
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{
			if (endNode && startNode) break;

			if (colCheck(totalNode[x][y]->rc, playerMove.rc)) // player��ġ ����
			{
				if (totalNode[x][y]->kind == TERRAIN::WALL) continue;

				totalNode[x][y]->nodeState = NODESTATE::NODE_END;
				endNode = totalNode[x][y];
				playerMove.x = x;
				playerMove.y = y;

			}
			if (colCheck(totalNode[x][y]->rc, monsterMove.rc)) // monster��ġ ����
			{
				if (totalNode[x][y]->kind == TERRAIN::WALL) continue;
				
				totalNode[x][y]->nodeState = NODESTATE::NODE_START;
				startNode = totalNode[x][y];
				monsterMove.x = x;
				monsterMove.y = y;
			}


		}

	}
	
	//��(��ֹ�) ��� �����ϱ� (����, ������ �������� �������� ���ϰ� ����)
	if (INPUT->GetKeyDown(VK_RBUTTON) && startNode && endNode)
	{
		for (int y = 0; y < MAXTILE_HEIGHT; y++)
		{
			for (int x = 0; x < MAXTILE_WIDTH; x++)
			{
				if (PtInRect(&totalNode[x][y]->rc, _ptMouse))
				{
					//���۳��, ������� �������� ���ϰ� ����
					if (totalNode[x][y]->nodeState == NODESTATE::NODE_START) continue;
					if (totalNode[x][y]->nodeState == NODESTATE::NODE_END) continue;
					totalNode[x][y]->nodeState = NODESTATE::NODE_WALL;
				}
			}
		}
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
			//cout << finalList.size() << endl;
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

void astarManager::addOpenList(int idx, int idy)
{
	//����ó�� �ε��� �����ȿ��� �߰��� �� �־�� �Ѵ�
	
	if (idx < monsterMove.x - 30 || idx >= monsterMove.x + 30 || idy < monsterMove.y - 30 || idy >= monsterMove.y + 30) return;

	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL && totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL || totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;


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

HRESULT monster::init(tagTile* tile, const char* fileName , POINT _pos, float _speed, int _hp, const char* skillImgName, bool _isKnockBack )
{
	if (!tile)
	{
		isAstar = false;
	}
	else
	{
		isAstar = true;
		astar = new astarManager;
		astar->init(tile);
	}

	img = IMAGEMANAGER->findImage(fileName);

	if(skillImgName)
		skillImg = IMAGEMANAGER->findImage(skillImgName);
	isKnockBack = _isKnockBack;
	cul = { 0,0 };
	pos.x = _pos.x;
	pos.y = _pos.y;
	speed = _speed;
	hp = _hp;
	frameIndex = { 0,0 };

	addInit();
	

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

void monster::hit(int damage , float hitAngle, float knockBack)
{
	hp -= damage;

	if (isKnockBack) // �з���.
	{
		pos.x += cos(hitAngle) * knockBack;
		pos.y += -sin(hitAngle) * knockBack;
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
		state = STATEIMAGE::DIE;
		isDie = true;
		
	}
}

