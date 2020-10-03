#include "stdafx.h"
#include "monster.h"

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
				totalNode[x][y]->nodeState = NODESTATE::NODE_END;
				endNode = totalNode[x][y];
				playerMove.x = x;
				playerMove.y = y;

			}
			if (colCheck(totalNode[x][y]->rc, monsterMove.rc)) // monster위치 설정
			{
				totalNode[x][y]->nodeState = NODESTATE::NODE_START;
				startNode = totalNode[x][y];
				monsterMove.x = x;
				monsterMove.y = y;
			}


		}

	}
	
	//벽(장애물) 노드 세팅하기 (시작, 종료노드 설정전에 벽세우지 못하게 막기)
	if (INPUT->GetKeyDown(VK_RBUTTON) && startNode && endNode)
	{
		for (int y = 0; y < MAXTILE_HEIGHT; y++)
		{
			for (int x = 0; x < MAXTILE_WIDTH; x++)
			{
				if (PtInRect(&totalNode[x][y]->rc, _ptMouse))
				{
					//시작노드, 종료노드는 선택하지 못하게 막기
					if (totalNode[x][y]->nodeState == NODESTATE::NODE_START) continue;
					if (totalNode[x][y]->nodeState == NODESTATE::NODE_END) continue;
					totalNode[x][y]->nodeState = NODESTATE::NODE_WALL;
				}
			}
		}
	}
	

	
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
	
	if (idx < monsterMove.x - 30 || idx >= monsterMove.x + 30 || idy < monsterMove.y - 30 || idy >= monsterMove.y + 30) return;

	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL && totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;
	if (totalNode[curNode->idx][idy]->kind == TERRAIN::WALL || totalNode[idx][curNode->idy]->kind == TERRAIN::WALL) return;


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
