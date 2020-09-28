#include "stdafx.h"
#include "astarTestScene.h"

HRESULT astarTestScene::init()
{
	//��� �ʱ�ȭ
	_startNode = NULL;
	_endNode = NULL;
	_curNode = NULL;

	//��ü��� �ʱ�ȭ
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			//���ο� ���� ��Ʈ��ġ ����
			_totalNode[x][y] = new node(x, y);
			_totalNode[x][y]->rc = RectMake(25 + x * 150, 25 + y * 150, 150, 150);
		}
	}

	//ùŬ���� ¦���� �ǰ� �ϱ� ���ؼ� -1�� �ʱ�ȭ
	_count = -1;
	//�� ã�ҳ�?
	_isFind = false;

	//����ŸƮ��
	_openList.clear();
	_closeList.clear();
	_finalList.clear();

	playerMove.x = _totalNode[0][0]->rc.left;
	playerMove.y = _totalNode[0][0]->rc.top;
	playerMove.speed = 5.f;
	
	
	

	_totalNode[0][0]->nodeState = NODE_START;
	_startNode = _totalNode[0][0];
	endPos.indexX = MAX_X-1;
	endPos.indexY = MAX_Y-1;
	_totalNode[endPos.indexX][endPos.indexY]->nodeState = NODE_END;
	_endNode = _totalNode[endPos.indexX][endPos.indexY];

	//playerMove.angle = getAngle(playerMove.x, playerMove.y, _endNode->rc.left, _endNode->rc.top);
	return S_OK;
}

void astarTestScene::release()
{
}

void astarTestScene::update()
{
	if (INPUT->GetKeyDown(VK_UP))
	{		
		if(endPos.indexY >0) 
			endPos.indexY -= 1;
		_openList.clear();
		_closeList.clear();
		_finalList.clear();
	}
	if (INPUT->GetKeyDown(VK_DOWN))
	{
		if (endPos.indexY < 4)
			endPos.indexY += 1;
		_openList.clear();
		_closeList.clear();
		_finalList.clear();
	}

	if (INPUT->GetKeyDown(VK_LEFT))
	{
		if (endPos.indexX > 0)
			endPos.indexX -= 1;
		_openList.clear();
		_closeList.clear();
		_finalList.clear();
	}
	if (INPUT->GetKeyDown(VK_RIGHT))
	{
		if (endPos.indexX < 4)
			endPos.indexX += 1;
		_openList.clear();
		_closeList.clear();
		_finalList.clear();
	}

	
		
		this->pathFinding();

		
		if (!_finalList.empty())
		{
			for (int i = 0; i < _finalList.size(); i++)
			{
				
				playerMove.angle = getAngle(playerMove.x, playerMove.y, _finalList[i]->rc.left, _finalList[i]->rc.top);
				break;
			}
			
		}
		RECT temp;
		if (!IntersectRect(&temp, &player, &monster))
		{
			playerMove.x += cos(playerMove.angle) * playerMove.speed;
			playerMove.y += -sin(playerMove.angle) * playerMove.speed;
		}
	

	


	_totalNode[endPos.indexX][endPos.indexY]->nodeState = NODE_END;
	_endNode = _totalNode[endPos.indexX][endPos.indexY];


		if (!_finalList.empty())
			_startNode = _finalList[0];
	/*if (INPUT->GetKeyDown(VK_SPACE))
	{
	}*/

	//����, ������ �����ϱ�
	//if (INPUT->GetKeyDown(VK_LBUTTON))
	//{
	//	//����, �����尡 �̹� ���õǾ� �ִٸ� �׳� ������
	//	if (_startNode && _endNode) return;

	//	for (int y = 0; y < MAX_Y; y++)
	//	{
	//		for (int x = 0; x < MAX_X; x++)
	//		{
	//			if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
	//			{
	//				_count++;
	//				if (_count % 2 == 0)
	//				{
	//					_totalNode[x][y]->nodeState = NODE_START;
	//					_startNode = _totalNode[x][y];
	//				}
	//				else
	//				{
	//					_totalNode[x][y]->nodeState = NODE_END;
	//					_endNode = _totalNode[x][y];
	//				}
	//			}
	//		}
	//	}
	//}

	

	//��(��ֹ�) ��� �����ϱ� (����, ������ �������� �������� ���ϰ� ����)
	if (INPUT->GetKeyDown(VK_RBUTTON) && _startNode && _endNode)
	{
		for (int y = 0; y < MAX_Y; y++)
		{
			for (int x = 0; x < MAX_X; x++)
			{
				if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
				{
					//���۳��, ������� �������� ���ϰ� ����
					if (_totalNode[x][y]->nodeState == NODE_START) continue;
					if (_totalNode[x][y]->nodeState == NODE_END) continue;
					_totalNode[x][y]->nodeState = NODE_WALL;
				}
			}
		}
	}

	

	//����ۿ�
	if (INPUT->GetKeyDown(VK_RETURN) && _isFind)
	{
		this->init();
	}

}

void astarTestScene::render()
{
	char str[128];
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			//��ü��� ��Ʈ �����ֱ�
			Rectangle(getMemDC(), _totalNode[x][y]->rc);

			//���۳�� �����ֱ�
			if (_totalNode[x][y]->nodeState == NODE_START)
			{
				setNodeColor(_startNode, RGB(255, 0, 0));
				textOut(getMemDC(), _startNode->rc.left + 100, _startNode->rc.top + 10, "[Start]",RGB(0,0,0));
			}
			//������ �����ֱ�
			if (_totalNode[x][y]->nodeState == NODE_END)
			{
				setNodeColor(_endNode, RGB(0, 0, 255));
				textOut(getMemDC(), _endNode->rc.left + 100, _endNode->rc.top + 10, "[END]");
			}
			//����� �����ֱ�
			if (_totalNode[x][y]->nodeState == NODE_WALL)
			{
				setNodeColor(_totalNode[x][y], RGB(200, 150, 100));
				textOut(getMemDC(), _totalNode[x][y]->rc.left + 100, _totalNode[x][y]->rc.top + 10, "[WALL]");
			}

			//��ü����� �ε��� �����ֱ�(�Ǹ������� ���)
			sprintf(str, "[%d, %d]", _totalNode[x][y]->idx, _totalNode[x][y]->idy);
			textOut(getMemDC(), _totalNode[x][y]->rc.left + 10, _totalNode[x][y]->rc.top + 10, str);
		}
	}

	//��ã������ �����ֱ�
	if (_isFind)
	{
		for (int i = 0; i < _finalList.size(); i++)
		{
			setNodeColor(_finalList[0], RGB(255, 255, 0));
			
			//_finalList[0]->nodeState = NODE_START;
			//
			//
			//_totalNode[0][0]->nodeState = NODE_EMPTY;
			//_startNode = _finalList[0];
			sprintf(str, "[%d, %d]      %d�� ���", _finalList[i]->idx, _finalList[i]->idy, i + 1);
			textOut(getMemDC(), _finalList[i]->rc.left + 10, _finalList[i]->rc.top + 10, str,RGB(0,0,0));
			sprintf(str, "G: %d, H: %d, F: %d", _finalList[i]->G, _finalList[i]->H, _finalList[i]->F);
			textOut(getMemDC(), _finalList[i]->rc.left + 10, _finalList[i]->rc.bottom - 30, str,RGB(0,0,0));
			break;
		}
	}



	player = RectMake(playerMove.x, playerMove.y, 50, 50);
	monster = RectMake(_endNode->rc.left, _endNode->rc.top, 50, 50);
	Rectangle(getMemDC(), player);
	Rectangle(getMemDC(), monster);
}

void astarTestScene::pathFinding()
{
	//�����尡 ���� ��� ��ã�� ����
	if (!_endNode) return;

	//��ã�⸦ �غ���
	//�˻��� �Ϸ��� ������ ���¸���Ʈ�� ��´�
	//F�� H�� ���� ���� ���� ã�Ƽ� �׳��� ������� �����Ѵ�
	//���¸���Ʈ���� ������� �����ϰ�
	//������� Ŭ�����Ʈ�� ��Ƶд�
	//���� �� ã�Ҵٸ� Ŭ�����Ʈ ���������� ���̳� ����Ʈ�� �ű��

	//1. ���۳�尡 �־�� ����� �����ϴ�
	//���۳�带 ���¸���Ʈ�� �߰��� ����� �Ѵ�
	_openList.push_back(_startNode);

	//2. ���¸���Ʈ�ȿ� ��� �ִ� ���͸� �˻��ؼ�
	//�����忡 �����Ҷ����� ���� ����
	while (_openList.size() > 0)
	{
		_curNode = _openList[0];

		//���¸���Ʈ�� F�� ���� �۰ų� F�� ���ٸ�
		//H�� ���� �� ������� �ϰ�
		//�����带 ���¸���Ʈ���� Ŭ���� ����Ʈ�� �ű��
		//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
		//��ã�Ⱑ ���۵ȴ�

		for (int i = 1; i < _openList.size(); i++)
		{
			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
			{
				_curNode = _openList[i];
			}
		}

		//Ŭ���� ����Ʈ�� �־�д�
		for (int i = 0; i < _openList.size(); i++)
		{
			if (_curNode == _openList[i])
			{
				this->delOpenList(i);
				_closeList.push_back(_curNode);
			}
		}

		//�����尡 ������ ���� ����? (��ã�Ҵ�)
		if (_curNode == _endNode)
		{
			node* endNode = _endNode;
			vector<node*> tempNode;
			//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
			while (endNode != _startNode)
			{
				tempNode.push_back(endNode);
				endNode = endNode->parentNode;
			}

			for (int i = tempNode.size() - 1; i > 0; i--)
			{
				_finalList.push_back(tempNode[i]);
			}

			_isFind = true;
			//�����ϰ� ���� ���´�
			return;
		}

		
		//�밢��
		addOpenList(_curNode->idx + 1, _curNode->idy+1);	//����
		addOpenList(_curNode->idx -1, _curNode->idy + 1);	//����
		addOpenList(_curNode->idx -1, _curNode->idy - 1);	//�»�
		addOpenList(_curNode->idx + 1, _curNode->idy-1);	//���
		
		//�����¿� (������ ������� - ��¥�� �ֺ� 4���� ��带 ��� ���¸���Ʈ�� ��Ƽ� �˻��� ������)

		addOpenList(_curNode->idx, _curNode->idy - 1);	//��
		addOpenList(_curNode->idx, _curNode->idy + 1);	//��
		addOpenList(_curNode->idx - 1, _curNode->idy);	//��
		addOpenList(_curNode->idx + 1, _curNode->idy);	//��

		//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
		//���, �»�, ����, ����
		//����ó���� �����ָ� �ȴ�
		//�����̷� ���� �ȵǵ��� ó���Ѵ�
	}

}

void astarTestScene::addOpenList(int idx, int idy)
{
	//����ó�� �ε��� �����ȿ��� �߰��� �� �־�� �Ѵ�
	if (idx < 0 || idx >= MAX_X || idy < 0 || idy >= MAX_Y) return;

	if (_totalNode[_curNode->idx][idy]->nodeState == NODE_WALL && _totalNode[idx][_curNode->idy]->nodeState == NODE_WALL) return;
	if (_totalNode[_curNode->idx][idy]->nodeState == NODE_WALL || _totalNode[idx][_curNode->idy]->nodeState == NODE_WALL) return;
	
	
	//���� ���¸���Ʈ�� ���� �� ����
	if (_totalNode[idx][idy]->nodeState == NODE_WALL) return;



	//Ŭ�����Ʈ(�����±�)�� �ִٸ� ���¸���Ʈ�� ������ �ȵȴ�
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idx][idy] == _closeList[i]) return;
	}

	//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
	//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��Ѵ�
	node* neighborNode = _totalNode[idx][idy];
	int moveCost = _curNode->G + ((_curNode->idx - idx == 0 || _curNode->idy - idy == 0) ? 10 : 14);

	//���¸���Ʈ�ȿ� �̿���尡 ������ �ȵȴ�
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == neighborNode) return;
	}

	//���������� ���¸���Ʈ���� ���°��
	//G, H, ParentNode ������ ���¸���Ʈ�� �߰�
	//F = G + H
	//G = ���ۿ��� ����
	//H = ���翡�� ����
	neighborNode->G = moveCost;
	neighborNode->H = (abs(neighborNode->idx - _endNode->idx) + abs(neighborNode->idy - _endNode->idy)) * 10;
	neighborNode->F = neighborNode->G + neighborNode->H;
	neighborNode->parentNode = _curNode;
	_openList.push_back(neighborNode);
}

void astarTestScene::delOpenList(int index)
{
	_openList.erase(_openList.begin() + index);
}

//���Ǹ� ���� �Լ�
void astarTestScene::setNodeColor(node * node, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(getMemDC(), &node->rc, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), node->rc, RGB(0, 0, 0));
}