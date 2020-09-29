#include "stdafx.h"
#include "astarManager.h"
//
HRESULT astarManager::init()
{

	

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
			//totalNode[x][y]->rc = RectMake(25 + x * 50, 25 + y * 50, 50, 50);
		}
	}

	//길 찾았냐?
	isFind = false;

	//벡터 초기화 (리스타트용)
	openList.clear();
	closeList.clear();
	finalList.clear();

	////몬스터
	//monsterMove.x = totalNode[0][0]->rc.left+10;
	//monsterMove.y = totalNode[0][0]->rc.top+10;
	//monsterMove.speed = 1.f;
	//
	//// 스타트 지점 지정
	//startPos.x = 0;
	//startPos.y = 0;
	//totalNode[startPos.x][startPos.y]->nodeState = NODESTATE::NODE_START;
	//startNode = totalNode[startPos.x][startPos.y];

	//// 도착지점 지정
	//endPos.x = MAXTILE_WIDTH-1;
	//endPos.y = 0;
	//totalNode[endPos.x][endPos.y]->nodeState = NODESTATE::NODE_END;
	//endNode = totalNode[endPos.x][endPos.y];

	////플레이어
	//playerMove.x = totalNode[endPos.x][endPos.y]->rc.left+10;
	//playerMove.y = totalNode[endPos.x][endPos.y]->rc.top+10;
	//playerMove.speed = 9.0f;


	return S_OK;
}

void astarManager::release()
{
}

void astarManager::update()
{
	/*if (INPUT->GetKeyDown(VK_UP))
	{		
		totalNode[endPos.x][endPos.y]->nodeState = NODESTATE::NODE_EMPTY;
		if(endPos.y >0) 
			endPos.y -= 1;
		
	}
	if (INPUT->GetKeyDown(VK_DOWN))
	{
		totalNode[endPos.x][endPos.y]->nodeState = NODESTATE::NODE_EMPTY;
		if (endPos.y < MAXTILE_HEIGHT-1)
			endPos.y += 1;
		
	}

	if (INPUT->GetKeyDown(VK_LEFT))
	{
		totalNode[endPos.x][endPos.y]->nodeState = NODESTATE::NODE_EMPTY;
		if (endPos.x > 0)
			endPos.x -= 1;
		
	}
	if (INPUT->GetKeyDown(VK_RIGHT))
	{
		totalNode[endPos.x][endPos.y]->nodeState = NODESTATE::NODE_EMPTY;
		if (endPos.x < MAXTILE_WIDTH-1)
			endPos.x += 1;
		
	}*/if (INPUT->GetKeyDown('O'))
	{
		for (int y = 0; y < MAXTILE_HEIGHT; y++)
		{
			for (int x = 0; x < MAXTILE_WIDTH; x++)
			{
				if (endNode && startNode) break;

				if (colCheck(totalNode[x][y]->rc, playerMove.rc))
				{
					totalNode[x][y]->nodeState = NODESTATE::NODE_END;
					endNode = totalNode[x][y];
					playerMove.x = x;
					playerMove.y = y;
					//playerMove.angle = getAngle(playerMove.x, playerMove.y, endNode->rc.left, endNode->rc.top);

				}
				if (colCheck(totalNode[x][y]->rc, monsterMove.rc))
				{
					totalNode[x][y]->nodeState = NODESTATE::NODE_START;
					startNode = totalNode[x][y];
					monsterMove.x = x;
					monsterMove.y = y;
				}


			}

		}
	}
	
	
	//
	//if (!colCheck(playerMove.rc, endNode->rc) )
	//{
	//	playerMove.x += cos( playerMove.angle) * playerMove.speed;
	//	playerMove.y += -sin(playerMove.angle) * playerMove.speed;
	//}



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
	//if (!finalList.empty())
	//{
	//	//if(MonsterMove.rc)
	//	//totalNode[startPos.x][startPos.y]->nodeState = NODESTATE::NODE_EMPTY;
	//	for (int i = 0; i < MAXTILE_HEIGHT; i++)
	//	{
	//		for (int k = 0; k < MAXTILE_WIDTH; k++)
	//		{
	//			if (totalNode[k][i] == finalList[0])
	//			{
	//				totalNode[k][i]->nodeState = NODESTATE::NODE_START;
	//				startNode = finalList[0];
	//				startPos.x = k;
	//				startPos.y = i;
	//				break;
	//			}
	//			
	//				
	//		}
	//	}
	//}

	if (INPUT->GetKeyDown('P'))
	{
		cout << 'p' << endl;
		this->pathFinding();
	}
	



	//if (!finalList.empty())
	//{
	//	monsterMove.angle = getAngle(monsterMove.x, monsterMove.y, finalList[0]->rc.left, finalList[0]->rc.top);
	//	
	//	if (colCheck( monsterMove.rc, finalList[0]->rc))
	//	{
	//		for (int i = 0; i < MAXTILE_HEIGHT; i++)
	//		{
	//			for (int k = 0; k < MAXTILE_WIDTH; k++)
	//			{
	//				if (totalNode[k][i] == finalList[0])
	//				{
	//					cout << "들어옴" << endl;
	//					totalNode[k][i]->nodeState = NODESTATE::NODE_START;
	//					startNode = totalNode[k][i];
	//					break;
	//				}
	//			}
	//		}

	//		openList.clear();
	//		closeList.clear();
	//		finalList.clear();

	//	}
	//	
	//}

	//
	//if (!finalList.empty())
	//{
	//		cout << "움직임" << endl;
	//		monsterMove.x += cos(monsterMove.angle) * monsterMove.speed;
	//		monsterMove.y += -sin(monsterMove.angle) * monsterMove.speed;
	//	//if (!IntersectRect(&temp, &playerMove.rc, &monsterMove.rc))
	//	//{
	//	//
	//	//}
	//}
	//else
	//{
	//	cout << "멈춤" << endl;
	//	openList.clear();
	//	closeList.clear();
	//	finalList.clear();
	//}
	//

	


	


		
	

	

	//재시작용
	if (INPUT->GetKeyDown(VK_RETURN) && isFind)
	{
		this->init();
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
			else FrameRect(hdc, totalNode[x][y]->rc,RGB(0,0,0));


			//시작노드 보여주기
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_START)
			{
				setNodeColor(startNode, RGB(255, 0, 0),hdc);
				textOut(hdc, startNode->rc.left , startNode->rc.top + 10, "[Start]",RGB(0,0,0));
			}
			//종료노드 보여주기
			if (totalNode[x][y] ->nodeState == NODESTATE::NODE_END)
			{
				setNodeColor(endNode, RGB(0, 0, 255),hdc);
				textOut(hdc, endNode->rc.left , endNode->rc.top + 20, "[END]");
			}
			//벽노드 보여주기
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_WALL)
			{
				setNodeColor(totalNode[x][y], RGB(200, 150, 100),hdc);
				textOut(hdc, totalNode[x][y]->rc.left + 100, totalNode[x][y]->rc.top + 10, "[WALL]");
			}

			//전체노드의 인덱스 보여주기(맨마지막에 출력)
			//sprintf(str, "[%d,%d]", totalNode[x][y]->idx, totalNode[x][y]->idy);
			//textOut(hdc, totalNode[x][y]->rc.left , totalNode[x][y]->rc.top , str,RGB(0,0,0));
		}
	}

	//길찾았을때 보여주기
	if (isFind)
	{
		for (int i = 0; i < finalList.size(); i++)
		{
			//cout << finalList.size() << endl;
			setNodeColor(finalList[i], RGB(255, 255, 0),hdc);
			//sprintf(str, "[%d, %d]      %d번 노드", finalList[i]->idx, finalList[i]->idy, i + 1);
			//textOut(hdc, finalList[i]->rc.left + 10, finalList[i]->rc.top + 10, str,RGB(0,0,0));
			//sprintf(str, "G: %d, H: %d, F: %d", finalList[i]->G, finalList[i]->H, finalList[i]->F);
			//textOut(hdc, finalList[i]->rc.left + 10, finalList[i]->rc.bottom - 30, str,RGB(0,0,0));
			
		}
		//setNodeColor(endNode, RGB(0, 0, 255),hdc);
	}


	//playerMove.rc = RectMake(playerMove.x, playerMove.y, 50, 50);
	//monsterMove.rc = RectMake(monsterMove.x, monsterMove.y, 50, 50);
	
	//Rectangle(hdc, startNode->rc);
	
	/*HBRUSH hbr = CreateSolidBrush(RGB(0, 0, 255));
	FillRect(hdc, &playerMove.rc,hbr);*/
	FrameRect(hdc, playerMove.rc, RGB(0, 0, 255));
	//Rectangle(hdc, player);
}

void astarManager::pathFinding()
{
	//종료노드가 없는 경우 길찾기 못함
	if (!endNode)  return; 

	//길찾기를 해보자
	//검색을 하려면 무조건 오픈리스트에 담는다
	//F와 H값 가장 작은 놈을 찾아서 그놈을 현재노드로 변경한다
	//오픈리스트에서 현재노드는 삭제하고
	//현재노드는 클로즈리스트에 담아둔다
	//길을 다 찾았다면 클로즈리스트 리버스값을 파이널 리스트로 옮긴다

	//1. 시작노드가 있어야 출발이 가능하니
	//시작노드를 오픈리스트에 추가를 해줘야 한다
	openList.push_back(startNode);

	//2. 오픈리스트안에 담겨 있는 벡터를 검사해서
	//종료노드에 도착할때까지 무한 루프
	while (openList.size() > 0)
	{
		curNode = openList[0];
		
		//오픈리스트중 F가 가장 작거나 F가 같다면
		//H가 작은 걸 현재노드로 하고
		//현재노드를 오픈리스트에서 클로즈 리스트로 옮기기
		//비교를 하려고 하면 최소 시작노드에서 주변을 탐색한 이후
		//길찾기가 시작된다

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
			cout << "길찾았냐" << endl;
			
			tileNode* _endNode = endNode;
			vector<tileNode*> tempNode;
			//마지막 노드로부터 시작노드까지 부모노드를 벡터에 담는다
			while (_endNode != startNode)
			{
				cout <<"x : "<< _endNode->idx << endl;
				cout <<"y : "<< _endNode->idy << endl;
				
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

		cout << "추가할 거 찾자" << endl;
		//대각선
		addOpenList( curNode->idx + 1,  curNode->idy+1);	//우하
		addOpenList( curNode->idx -1,  curNode->idy + 1);	//좌하
		addOpenList( curNode->idx -1,  curNode->idy - 1);	//좌상
		addOpenList( curNode->idx + 1,  curNode->idy-1);	//우상
		
		//상하좌우 (순서는 상관없음 - 어짜피 주변 4개의 노드를 모두 오픈리스트에 담아서 검사할 예정임)

		addOpenList( curNode->idx,  curNode->idy - 1);	//상
		addOpenList( curNode->idx,  curNode->idy + 1);	//하
		addOpenList( curNode->idx - 1,  curNode->idy);	//좌
		addOpenList( curNode->idx + 1,  curNode->idy);	//우

		//추후에 대각 4방향도 추가하면 대각선 이동 처리도 가능함
		//우상, 좌상, 우하, 좌하
		//예외처리만 잘해주면 된다
		//벽사이로 막가 안되도록 처리한다
	}

}

void astarManager::addOpenList(int idx, int idy)
{
	//예외처리 인덱스 범위안에서 추가할 수 있어야 한다
	cout << "오픈 추가 생존" << endl;
	cout << idx << endl;
	cout << idy << endl;
	cout << "playerMove X : " <<playerMove.x - 20 << endl;
	cout << "playerMove Y : " <<playerMove.y - 20 << endl;
	if (idx < monsterMove.x-20 || idx >= monsterMove.x+20 || idy <monsterMove.y-20 || idy >= monsterMove.y+20) return;

	if (totalNode[ curNode->idx][idy]->kind == TERRAIN::WALL && totalNode[idx][ curNode->idy]->kind ==  TERRAIN::WALL) return;
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

	//마지막으로 오픈리스트에도 없는경우
	//G, H, ParentNode 설정후 오픈리스트에 추가
	//F = G + H
	//G = 시작에서 현재
	//H = 현재에서 종료
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
