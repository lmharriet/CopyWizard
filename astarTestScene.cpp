#include "stdafx.h"
#include "astarTestScene.h"

HRESULT astarTestScene::init()
{
	//노드 초기화
	_startNode = NULL;
	_endNode = NULL;
	_curNode = NULL;

	//전체노드 초기화
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			//새로운 노드와 렉트위치 설정
			_totalNode[x][y] = new node(x, y);
			_totalNode[x][y]->rc = RectMake(25 + x * 150, 25 + y * 150, 150, 150);
		}
	}

	//첫클릭이 짝수가 되게 하기 위해서 -1로 초기화
	_count = -1;
	//길 찾았냐?
	_isFind = false;

	//리스타트용
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

	//시작, 종료노드 세팅하기
	//if (INPUT->GetKeyDown(VK_LBUTTON))
	//{
	//	//시작, 종료노드가 이미 세팅되어 있다면 그냥 나가자
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

	

	//벽(장애물) 노드 세팅하기 (시작, 종료노드 설정전에 벽세우지 못하게 막기)
	if (INPUT->GetKeyDown(VK_RBUTTON) && _startNode && _endNode)
	{
		for (int y = 0; y < MAX_Y; y++)
		{
			for (int x = 0; x < MAX_X; x++)
			{
				if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
				{
					//시작노드, 종료노드는 선택하지 못하게 막기
					if (_totalNode[x][y]->nodeState == NODE_START) continue;
					if (_totalNode[x][y]->nodeState == NODE_END) continue;
					_totalNode[x][y]->nodeState = NODE_WALL;
				}
			}
		}
	}

	

	//재시작용
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
			//전체노드 렉트 보여주기
			Rectangle(getMemDC(), _totalNode[x][y]->rc);

			//시작노드 보여주기
			if (_totalNode[x][y]->nodeState == NODE_START)
			{
				setNodeColor(_startNode, RGB(255, 0, 0));
				textOut(getMemDC(), _startNode->rc.left + 100, _startNode->rc.top + 10, "[Start]",RGB(0,0,0));
			}
			//종료노드 보여주기
			if (_totalNode[x][y]->nodeState == NODE_END)
			{
				setNodeColor(_endNode, RGB(0, 0, 255));
				textOut(getMemDC(), _endNode->rc.left + 100, _endNode->rc.top + 10, "[END]");
			}
			//벽노드 보여주기
			if (_totalNode[x][y]->nodeState == NODE_WALL)
			{
				setNodeColor(_totalNode[x][y], RGB(200, 150, 100));
				textOut(getMemDC(), _totalNode[x][y]->rc.left + 100, _totalNode[x][y]->rc.top + 10, "[WALL]");
			}

			//전체노드의 인덱스 보여주기(맨마지막에 출력)
			sprintf(str, "[%d, %d]", _totalNode[x][y]->idx, _totalNode[x][y]->idy);
			textOut(getMemDC(), _totalNode[x][y]->rc.left + 10, _totalNode[x][y]->rc.top + 10, str);
		}
	}

	//길찾았을때 보여주기
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
			sprintf(str, "[%d, %d]      %d번 노드", _finalList[i]->idx, _finalList[i]->idy, i + 1);
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
	//종료노드가 없는 경우 길찾기 못함
	if (!_endNode) return;

	//길찾기를 해보자
	//검색을 하려면 무조건 오픈리스트에 담는다
	//F와 H값 가장 작은 놈을 찾아서 그놈을 현재노드로 변경한다
	//오픈리스트에서 현재노드는 삭제하고
	//현재노드는 클로즈리스트에 담아둔다
	//길을 다 찾았다면 클로즈리스트 리버스값을 파이널 리스트로 옮긴다

	//1. 시작노드가 있어야 출발이 가능하니
	//시작노드를 오픈리스트에 추가를 해줘야 한다
	_openList.push_back(_startNode);

	//2. 오픈리스트안에 담겨 있는 벡터를 검사해서
	//종료노드에 도착할때까지 무한 루프
	while (_openList.size() > 0)
	{
		_curNode = _openList[0];

		//오픈리스트중 F가 가장 작거나 F가 같다면
		//H가 작은 걸 현재노드로 하고
		//현재노드를 오픈리스트에서 클로즈 리스트로 옮기기
		//비교를 하려고 하면 최소 시작노드에서 주변을 탐색한 이후
		//길찾기가 시작된다

		for (int i = 1; i < _openList.size(); i++)
		{
			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
			{
				_curNode = _openList[i];
			}
		}

		//클로즈 리스트에 넣어둔다
		for (int i = 0; i < _openList.size(); i++)
		{
			if (_curNode == _openList[i])
			{
				this->delOpenList(i);
				_closeList.push_back(_curNode);
			}
		}

		//현재노드가 마지막 노드와 같냐? (길찾았다)
		if (_curNode == _endNode)
		{
			node* endNode = _endNode;
			vector<node*> tempNode;
			//마지막 노드로부터 시작노드까지 부모노드를 벡터에 담는다
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
			//종료하고 빠져 나온다
			return;
		}

		
		//대각선
		addOpenList(_curNode->idx + 1, _curNode->idy+1);	//우하
		addOpenList(_curNode->idx -1, _curNode->idy + 1);	//좌하
		addOpenList(_curNode->idx -1, _curNode->idy - 1);	//좌상
		addOpenList(_curNode->idx + 1, _curNode->idy-1);	//우상
		
		//상하좌우 (순서는 상관없음 - 어짜피 주변 4개의 노드를 모두 오픈리스트에 담아서 검사할 예정임)

		addOpenList(_curNode->idx, _curNode->idy - 1);	//상
		addOpenList(_curNode->idx, _curNode->idy + 1);	//하
		addOpenList(_curNode->idx - 1, _curNode->idy);	//좌
		addOpenList(_curNode->idx + 1, _curNode->idy);	//우

		//추후에 대각 4방향도 추가하면 대각선 이동 처리도 가능함
		//우상, 좌상, 우하, 좌하
		//예외처리만 잘해주면 된다
		//벽사이로 막가 안되도록 처리한다
	}

}

void astarTestScene::addOpenList(int idx, int idy)
{
	//예외처리 인덱스 범위안에서 추가할 수 있어야 한다
	if (idx < 0 || idx >= MAX_X || idy < 0 || idy >= MAX_Y) return;

	if (_totalNode[_curNode->idx][idy]->nodeState == NODE_WALL && _totalNode[idx][_curNode->idy]->nodeState == NODE_WALL) return;
	if (_totalNode[_curNode->idx][idy]->nodeState == NODE_WALL || _totalNode[idx][_curNode->idy]->nodeState == NODE_WALL) return;
	
	
	//벽은 오픈리스트에 담을 수 없다
	if (_totalNode[idx][idy]->nodeState == NODE_WALL) return;



	//클로즈리스트(지나온길)에 있다면 오픈리스트에 담으면 안된다
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idx][idy] == _closeList[i]) return;
	}

	//여기까지 왔으면 문제가 없으니 이제 오픈리스트에 추가를 하자
	//현재노드의 4방향 노드를 이웃노드라고 하고 직선10, 대각은 14비용을 추가한다
	node* neighborNode = _totalNode[idx][idy];
	int moveCost = _curNode->G + ((_curNode->idx - idx == 0 || _curNode->idy - idy == 0) ? 10 : 14);

	//오픈리스트안에 이웃노드가 있으면 안된다
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == neighborNode) return;
	}

	//마지막으로 오픈리스트에도 없는경우
	//G, H, ParentNode 설정후 오픈리스트에 추가
	//F = G + H
	//G = 시작에서 현재
	//H = 현재에서 종료
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

//편의를 위한 함수
void astarTestScene::setNodeColor(node * node, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(getMemDC(), &node->rc, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), node->rc, RGB(0, 0, 0));
}