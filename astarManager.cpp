#include "stdafx.h"
#include "astarManager.h"
//
HRESULT astarManager::init()
{

	

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
			//totalNode[x][y]->rc = RectMake(25 + x * 50, 25 + y * 50, 50, 50);
		}
	}

	//�� ã�ҳ�?
	isFind = false;

	//���� �ʱ�ȭ (����ŸƮ��)
	openList.clear();
	closeList.clear();
	finalList.clear();

	////����
	//monsterMove.x = totalNode[0][0]->rc.left+10;
	//monsterMove.y = totalNode[0][0]->rc.top+10;
	//monsterMove.speed = 1.f;
	//
	//// ��ŸƮ ���� ����
	//startPos.x = 0;
	//startPos.y = 0;
	//totalNode[startPos.x][startPos.y]->nodeState = NODESTATE::NODE_START;
	//startNode = totalNode[startPos.x][startPos.y];

	//// �������� ����
	//endPos.x = MAXTILE_WIDTH-1;
	//endPos.y = 0;
	//totalNode[endPos.x][endPos.y]->nodeState = NODESTATE::NODE_END;
	//endNode = totalNode[endPos.x][endPos.y];

	////�÷��̾�
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
	//					cout << "����" << endl;
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
	//		cout << "������" << endl;
	//		monsterMove.x += cos(monsterMove.angle) * monsterMove.speed;
	//		monsterMove.y += -sin(monsterMove.angle) * monsterMove.speed;
	//	//if (!IntersectRect(&temp, &playerMove.rc, &monsterMove.rc))
	//	//{
	//	//
	//	//}
	//}
	//else
	//{
	//	cout << "����" << endl;
	//	openList.clear();
	//	closeList.clear();
	//	finalList.clear();
	//}
	//

	


	


		
	

	

	//����ۿ�
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
			

			//  ���緺Ʈ�� �����ֱ�
			if (colCheck(cam, totalNode[x][y]->rc) == false || totalNode[x][y]->keyName == "") continue;
			
			if (totalNode[x][y]->kind == TERRAIN::WALL) setNodeColor(totalNode[x][y], RGB(250, 150, 0), hdc);
			else FrameRect(hdc, totalNode[x][y]->rc,RGB(0,0,0));


			//���۳�� �����ֱ�
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_START)
			{
				setNodeColor(startNode, RGB(255, 0, 0),hdc);
				textOut(hdc, startNode->rc.left , startNode->rc.top + 10, "[Start]",RGB(0,0,0));
			}
			//������ �����ֱ�
			if (totalNode[x][y] ->nodeState == NODESTATE::NODE_END)
			{
				setNodeColor(endNode, RGB(0, 0, 255),hdc);
				textOut(hdc, endNode->rc.left , endNode->rc.top + 20, "[END]");
			}
			//����� �����ֱ�
			if (totalNode[x][y]->nodeState == NODESTATE::NODE_WALL)
			{
				setNodeColor(totalNode[x][y], RGB(200, 150, 100),hdc);
				textOut(hdc, totalNode[x][y]->rc.left + 100, totalNode[x][y]->rc.top + 10, "[WALL]");
			}

			//��ü����� �ε��� �����ֱ�(�Ǹ������� ���)
			sprintf(str, "[%d,%d]", totalNode[x][y]->idx, totalNode[x][y]->idy);
			textOut(hdc, totalNode[x][y]->rc.left , totalNode[x][y]->rc.top , str,RGB(0,0,0));
		}
	}

	//��ã������ �����ֱ�
	if (isFind)
	{
		for (int i = 0; i < finalList.size(); i++)
		{
			//cout << finalList.size() << endl;
			setNodeColor(finalList[i], RGB(255, 255, 0),hdc);
			sprintf(str, "[%d, %d]      %d�� ���", finalList[i]->idx, finalList[i]->idy, i + 1);
			textOut(hdc, finalList[i]->rc.left + 10, finalList[i]->rc.top + 10, str,RGB(0,0,0));
			sprintf(str, "G: %d, H: %d, F: %d", finalList[i]->G, finalList[i]->H, finalList[i]->F);
			textOut(hdc, finalList[i]->rc.left + 10, finalList[i]->rc.bottom - 30, str,RGB(0,0,0));
			
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
	//�����尡 ���� ��� ��ã�� ����
	if (!endNode)  return; 

	//��ã�⸦ �غ���
	//�˻��� �Ϸ��� ������ ���¸���Ʈ�� ��´�
	//F�� H�� ���� ���� ���� ã�Ƽ� �׳��� ������� �����Ѵ�
	//���¸���Ʈ���� ������� �����ϰ�
	//������� Ŭ�����Ʈ�� ��Ƶд�
	//���� �� ã�Ҵٸ� Ŭ�����Ʈ ���������� ���̳� ����Ʈ�� �ű��

	//1. ���۳�尡 �־�� ����� �����ϴ�
	//���۳�带 ���¸���Ʈ�� �߰��� ����� �Ѵ�
	openList.push_back(startNode);

	//2. ���¸���Ʈ�ȿ� ��� �ִ� ���͸� �˻��ؼ�
	//�����忡 �����Ҷ����� ���� ����
	while (openList.size() > 0)
	{
		curNode = openList[0];
		
		//���¸���Ʈ�� F�� ���� �۰ų� F�� ���ٸ�
		//H�� ���� �� ������� �ϰ�
		//�����带 ���¸���Ʈ���� Ŭ���� ����Ʈ�� �ű��
		//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
		//��ã�Ⱑ ���۵ȴ�

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
			cout << "��ã�ҳ�" << endl;
			
			tileNode* _endNode = endNode;
			vector<tileNode*> tempNode;
			//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
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
			//�����ϰ� ���� ���´�
			return;
		}

		cout << "�߰��� �� ã��" << endl;
		//�밢��
		addOpenList( curNode->idx + 1,  curNode->idy+1);	//����
		addOpenList( curNode->idx -1,  curNode->idy + 1);	//����
		addOpenList( curNode->idx -1,  curNode->idy - 1);	//�»�
		addOpenList( curNode->idx + 1,  curNode->idy-1);	//���
		
		//�����¿� (������ ������� - ��¥�� �ֺ� 4���� ��带 ��� ���¸���Ʈ�� ��Ƽ� �˻��� ������)

		addOpenList( curNode->idx,  curNode->idy - 1);	//��
		addOpenList( curNode->idx,  curNode->idy + 1);	//��
		addOpenList( curNode->idx - 1,  curNode->idy);	//��
		addOpenList( curNode->idx + 1,  curNode->idy);	//��

		//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
		//���, �»�, ����, ����
		//����ó���� �����ָ� �ȴ�
		//�����̷� ���� �ȵǵ��� ó���Ѵ�
	}

}

void astarManager::addOpenList(int idx, int idy)
{
	//����ó�� �ε��� �����ȿ��� �߰��� �� �־�� �Ѵ�
	cout << "���� �߰� ����" << endl;
	cout << idx << endl;
	cout << idy << endl;
	cout << "playerMove X : " <<playerMove.x - 20 << endl;
	cout << "playerMove Y : " <<playerMove.y - 20 << endl;
	if (idx < monsterMove.x-20 || idx >= monsterMove.x+20 || idy <monsterMove.y-20 || idy >= monsterMove.y+20) return;

	if (totalNode[ curNode->idx][idy]->kind == TERRAIN::WALL && totalNode[idx][ curNode->idy]->kind ==  TERRAIN::WALL) return;
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

	//���������� ���¸���Ʈ���� ���°��
	//G, H, ParentNode ������ ���¸���Ʈ�� �߰�
	//F = G + H
	//G = ���ۿ��� ����
	//H = ���翡�� ����
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
