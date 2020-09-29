#include "stdafx.h"
#include "astarManager.h"
//
HRESULT astarManager::init()
{

	

	//��� �ʱ�ȭ
	_startNode = NULL;
	_endNode = NULL;
	_curNode = NULL;

	//��üŸ�� ��� �ʱ�ȭ
	for (int y = 0; y < MAXTILE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXTILE_WIDTH; x++)
		{
			//���ο� ���� ��Ʈ��ġ ����
			_totalNode[x][y] = new tileNode(x, y);
			_totalNode[x][y]->rc = RectMake(25 + x * 50, 25 + y * 50, 50, 50);
		}
	}

	//�� ã�ҳ�?
	_isFind = false;

	//���� �ʱ�ȭ (����ŸƮ��)
	_openList.clear();
	_closeList.clear();
	_finalList.clear();

	//����
	monsterMove.x = _totalNode[0][0]->rc.left+10;
	monsterMove.y = _totalNode[0][0]->rc.top+10;
	monsterMove.speed = 1.f;
	
	// ��ŸƮ ���� ����
	startPos.indexX = 0;
	startPos.indexY = 0;
	_totalNode[startPos.indexX][startPos.indexY]->kind = TERRAIN::NONE;
	_startNode = _totalNode[startPos.indexX][startPos.indexY];

	// �������� ����
	endPos.indexX = MAXTILE_WIDTH-1;
	endPos.indexY = 0;
	_totalNode[endPos.indexX][endPos.indexY]->kind = TERRAIN::NONE;
	_endNode = _totalNode[endPos.indexX][endPos.indexY];

	//�÷��̾�
	playerMove.x = _totalNode[endPos.indexX][endPos.indexY]->rc.left+10;
	playerMove.y = _totalNode[endPos.indexX][endPos.indexY]->rc.top+10;
	playerMove.speed = 9.0f;


	return S_OK;
}
//
//void astarManager::release()
//{
//}
//
//void astarManager::update()
//{
//	if (INPUT->GetKeyDown(VK_UP))
//	{		
//		_totalNode[endPos.indexX][endPos.indexY]->nodeState = NODE_EMPTY;
//		if(endPos.indexY >0) 
//			endPos.indexY -= 1;
//		
//	}
//	if (INPUT->GetKeyDown(VK_DOWN))
//	{
//		_totalNode[endPos.indexX][endPos.indexY]->nodeState = NODE_EMPTY;
//		if (endPos.indexY < MAXTILE_HEIGHT-1)
//			endPos.indexY += 1;
//		
//	}
//
//	if (INPUT->GetKeyDown(VK_LEFT))
//	{
//		_totalNode[endPos.indexX][endPos.indexY]->nodeState = NODE_EMPTY;
//		if (endPos.indexX > 0)
//			endPos.indexX -= 1;
//		
//	}
//	if (INPUT->GetKeyDown(VK_RIGHT))
//	{
//		_totalNode[endPos.indexX][endPos.indexY]->nodeState = NODE_EMPTY;
//		if (endPos.indexX < MAXTILE_WIDTH-1)
//			endPos.indexX += 1;
//		
//	}
//
//	_totalNode[endPos.indexX][endPos.indexY]->nodeState = NODE_END;
//	_endNode = _totalNode[endPos.indexX][endPos.indexY];
//	playerMove.angle = getAngle(playerMove.x, playerMove.y, _endNode->rc.left, _endNode->rc.top);
//	
//	
//	
//	if (!IntersectRect(&temp, &playerMove.rc, &_endNode->rc))
//	{
//		playerMove.x += cos( playerMove.angle) * playerMove.speed;
//		playerMove.y += -sin(playerMove.angle) * playerMove.speed;
//	}
//
//
//
//	//��(��ֹ�) ��� �����ϱ� (����, ������ �������� �������� ���ϰ� ����)
//	if (INPUT->GetKeyDown(VK_RBUTTON) && _startNode && _endNode)
//	{
//		for (int y = 0; y < MAXTILE_HEIGHT; y++)
//		{
//			for (int x = 0; x < MAXTILE_WIDTH; x++)
//			{
//				if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
//				{
//					//���۳��, ������� �������� ���ϰ� ����
//					if (_totalNode[x][y]->nodeState == NODE_START) continue;
//					if (_totalNode[x][y]->nodeState == NODE_END) continue;
//					_totalNode[x][y]->nodeState = NODE_WALL;
//				}
//			}
//		}
//	}
//	//if (!_finalList.empty())
//	//{
//	//	//if(MonsterMove.rc)
//	//	//_totalNode[startPos.indexX][startPos.indexY]->nodeState = NODE_EMPTY;
//	//	for (int i = 0; i < MAXTILE_HEIGHT; i++)
//	//	{
//	//		for (int k = 0; k < MAXTILE_WIDTH; k++)
//	//		{
//	//			if (_totalNode[k][i] == _finalList[0])
//	//			{
//	//				_totalNode[k][i]->nodeState = NODE_START;
//	//				_startNode = _finalList[0];
//	//				startPos.indexX = k;
//	//				startPos.indexY = i;
//	//				break;
//	//			}
//	//			
//	//				
//	//		}
//	//	}
//	//}
//
//	this->pathFinding();
//	
//
//
//
//	if (!_finalList.empty())
//	{
//		monsterMove.angle = getAngle(monsterMove.x, monsterMove.y, _finalList[0]->rc.left, _finalList[0]->rc.top);
//		
//		if (IntersectRect(&temp, &monsterMove.rc, &_finalList[0]->rc))
//		{
//			for (int i = 0; i < MAXTILE_HEIGHT; i++)
//			{
//				for (int k = 0; k < MAXTILE_WIDTH; k++)
//				{
//					if (_totalNode[k][i] == _finalList[0])
//					{
//						cout << "����" << endl;
//						_totalNode[k][i]->nodeState = NODE_START;
//						_startNode = _totalNode[k][i];
//						break;
//					}
//				}
//			}
//
//			_openList.clear();
//			_closeList.clear();
//			_finalList.clear();
//
//		}
//		
//	}
//
//	
//	if (!_finalList.empty())
//	{
//			cout << "������" << endl;
//			monsterMove.x += cos(monsterMove.angle) * monsterMove.speed;
//			monsterMove.y += -sin(monsterMove.angle) * monsterMove.speed;
//		//if (!IntersectRect(&temp, &playerMove.rc, &monsterMove.rc))
//		//{
//		//
//		//}
//	}
//	else
//	{
//		cout << "����" << endl;
//		_openList.clear();
//		_closeList.clear();
//		_finalList.clear();
//	}
//	
//
//	
//
//
//	
//
//
//		
//	
//
//	
//
//	//����ۿ�
//	if (INPUT->GetKeyDown(VK_RETURN) && _isFind)
//	{
//		this->init();
//	}
//
//}
//
//
//void astarManager::render()
//{
//	char str[128];
//	for (int y = 0; y < MAXTILE_HEIGHT; y++)
//	{
//		for (int x = 0; x < MAXTILE_WIDTH; x++)
//		{
//			//��ü��� ��Ʈ �����ֱ�
//			Rectangle(getMemDC(), _totalNode[x][y]->rc);
//
//			//���۳�� �����ֱ�
//			if (_totalNode[x][y]->nodeState == NODE_START)
//			{
//				setNodeColor(_startNode, RGB(255, 0, 0));
//				textOut(getMemDC(), _startNode->rc.left + 100, _startNode->rc.top + 10, "[Start]",RGB(0,0,0));
//			}
//			//������ �����ֱ�
//			if (_totalNode[x][y] ->nodeState == NODE_END)
//			{
//				setNodeColor(_endNode, RGB(0, 0, 255));
//				textOut(getMemDC(), _endNode->rc.left , _endNode->rc.top + 20, "[END]");
//			}
//			//����� �����ֱ�
//			if (_totalNode[x][y]->nodeState == NODE_WALL)
//			{
//				setNodeColor(_totalNode[x][y], RGB(200, 150, 100));
//				textOut(getMemDC(), _totalNode[x][y]->rc.left + 100, _totalNode[x][y]->rc.top + 10, "[WALL]");
//			}
//
//			//��ü����� �ε��� �����ֱ�(�Ǹ������� ���)
//			sprintf(str, "[%d, %d]", _totalNode[x][y]->idx, _totalNode[x][y]->idy);
//			textOut(getMemDC(), _totalNode[x][y]->rc.left + 10, _totalNode[x][y]->rc.top + 10, str,RGB(0,0,0));
//		}
//	}
//
//	//��ã������ �����ֱ�
//	//if (_isFind)
//	//{
//	//	for (int i = 0; i < _finalList.size(); i++)
//	//	{
//	//		//setNodeColor(_finalList[0], RGB(255, 255, 0));
//	//		//_finalList[0]->nodeState = NODE_START;
//	//		//_totalNode[0][0]->nodeState = NODE_EMPTY;
//	//		//_startNode = _finalList[0];
//	//		//sprintf(str, "[%d, %d]      %d�� ���", _finalList[i]->idx, _finalList[i]->idy, i + 1);
//	//		//textOut(getMemDC(), _finalList[i]->rc.left + 10, _finalList[i]->rc.top + 10, str,RGB(0,0,0));
//	//		//sprintf(str, "G: %d, H: %d, F: %d", _finalList[i]->G, _finalList[i]->H, _finalList[i]->F);
//	//		//textOut(getMemDC(), _finalList[i]->rc.left + 10, _finalList[i]->rc.bottom - 30, str,RGB(0,0,0));
//	//		break;
//	//	}
//	//	//setNodeColor(_endNode, RGB(0, 0, 255));
//	//}
//
//
//	playerMove.rc = RectMake(playerMove.x, playerMove.y, 50, 50);
//	monsterMove.rc = RectMake(monsterMove.x, monsterMove.y, 50, 50);
//	
//	Rectangle(getMemDC(), monsterMove.rc);
//	
//	HBRUSH hbr = CreateSolidBrush(RGB(0, 0, 255));
//	FillRect(getMemDC(), &playerMove.rc,hbr);
//	//Rectangle(getMemDC(), player);
//}
//
//void astarManager::pathFinding()
//{
//	//�����尡 ���� ��� ��ã�� ����
//	if (!_endNode) return;
//
//	//��ã�⸦ �غ���
//	//�˻��� �Ϸ��� ������ ���¸���Ʈ�� ��´�
//	//F�� H�� ���� ���� ���� ã�Ƽ� �׳��� ������� �����Ѵ�
//	//���¸���Ʈ���� ������� �����ϰ�
//	//������� Ŭ�����Ʈ�� ��Ƶд�
//	//���� �� ã�Ҵٸ� Ŭ�����Ʈ ���������� ���̳� ����Ʈ�� �ű��
//
//	//1. ���۳�尡 �־�� ����� �����ϴ�
//	//���۳�带 ���¸���Ʈ�� �߰��� ����� �Ѵ�
//	_openList.push_back(_startNode);
//
//	//2. ���¸���Ʈ�ȿ� ��� �ִ� ���͸� �˻��ؼ�
//	//�����忡 �����Ҷ����� ���� ����
//	while (_openList.size() > 0)
//	{
//		_curNode = _openList[0];
//
//		//���¸���Ʈ�� F�� ���� �۰ų� F�� ���ٸ�
//		//H�� ���� �� ������� �ϰ�
//		//�����带 ���¸���Ʈ���� Ŭ���� ����Ʈ�� �ű��
//		//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
//		//��ã�Ⱑ ���۵ȴ�
//
//		for (int i = 1; i < _openList.size(); i++)
//		{
//			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
//			{
//				_curNode = _openList[i];
//			}
//		}
//
//		//Ŭ���� ����Ʈ�� �־�д�
//		for (int i = 0; i < _openList.size(); i++)
//		{
//			if (_curNode == _openList[i])
//			{
//				this->delOpenList(i);
//				_closeList.push_back(_curNode);
//			}
//		}
//
//		//�����尡 ������ ���� ����? (��ã�Ҵ�)
//		if (_curNode == _endNode)
//		{
//			node* endNode = _endNode;
//			vector<node*> tempNode;
//			//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
//			while (endNode != _startNode)
//			{
//				tempNode.push_back(endNode);
//				endNode = endNode->parentNode;
//			}
//
//			for (int i = tempNode.size() - 1; i > 0; i--)
//			{
//				_finalList.push_back(tempNode[i]);
//			}
//
//			_isFind = true;
//			//�����ϰ� ���� ���´�
//			return;
//		}
//
//		
//		//�밢��
//		addOpenList(_curNode->idx + 1, _curNode->idy+1);	//����
//		addOpenList(_curNode->idx -1, _curNode->idy + 1);	//����
//		addOpenList(_curNode->idx -1, _curNode->idy - 1);	//�»�
//		addOpenList(_curNode->idx + 1, _curNode->idy-1);	//���
//		
//		//�����¿� (������ ������� - ��¥�� �ֺ� 4���� ��带 ��� ���¸���Ʈ�� ��Ƽ� �˻��� ������)
//
//		addOpenList(_curNode->idx, _curNode->idy - 1);	//��
//		addOpenList(_curNode->idx, _curNode->idy + 1);	//��
//		addOpenList(_curNode->idx - 1, _curNode->idy);	//��
//		addOpenList(_curNode->idx + 1, _curNode->idy);	//��
//
//		//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
//		//���, �»�, ����, ����
//		//����ó���� �����ָ� �ȴ�
//		//�����̷� ���� �ȵǵ��� ó���Ѵ�
//	}
//
//}
//
//void astarManager::addOpenList(int idx, int idy)
//{
//	//����ó�� �ε��� �����ȿ��� �߰��� �� �־�� �Ѵ�
//	if (idx < 0 || idx >= MAXTILE_WIDTH || idy < 0 || idy >= MAXTILE_HEIGHT) return;
//
//	if (_totalNode[_curNode->idx][idy]->nodeState == NODE_WALL && _totalNode[idx][_curNode->idy]->nodeState == NODE_WALL) return;
//	if (_totalNode[_curNode->idx][idy]->nodeState == NODE_WALL || _totalNode[idx][_curNode->idy]->nodeState == NODE_WALL) return;
//	
//	
//	//���� ���¸���Ʈ�� ���� �� ����
//	if (_totalNode[idx][idy]->nodeState == NODE_WALL) return;
//
//
//
//	//Ŭ�����Ʈ(�����±�)�� �ִٸ� ���¸���Ʈ�� ������ �ȵȴ�
//	for (int i = 0; i < _closeList.size(); i++)
//	{
//		if (_totalNode[idx][idy] == _closeList[i]) return;
//	}
//
//	//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
//	//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��Ѵ�
//	node* neighborNode = _totalNode[idx][idy];
//	int moveCost = _curNode->G + ((_curNode->idx - idx == 0 || _curNode->idy - idy == 0) ? 10 : 14);
//
//	//���¸���Ʈ�ȿ� �̿���尡 ������ �ȵȴ�
//	for (int i = 0; i < _openList.size(); i++)
//	{
//		if (_openList[i] == neighborNode) return;
//	}
//
//	//���������� ���¸���Ʈ���� ���°��
//	//G, H, ParentNode ������ ���¸���Ʈ�� �߰�
//	//F = G + H
//	//G = ���ۿ��� ����
//	//H = ���翡�� ����
//	neighborNode->G = moveCost;
//	neighborNode->H = (abs(neighborNode->idx - _endNode->idx) + abs(neighborNode->idy - _endNode->idy)) * 10;
//	neighborNode->F = neighborNode->G + neighborNode->H;
//	neighborNode->parentNode = _curNode;
//	_openList.push_back(neighborNode);
//}
//
//void astarManager::delOpenList(int index)
//{
//	_openList.erase(_openList.begin() + index);
//}
//
////���Ǹ� ���� �Լ�
//void astarManager::setNodeColor(node * node, COLORREF color)
//{
//	HBRUSH brush = CreateSolidBrush(color);
//	FillRect(getMemDC(), &node->rc, brush);
//	DeleteObject(brush);
//	FrameRect(getMemDC(), node->rc, RGB(0, 0, 0));
//}
