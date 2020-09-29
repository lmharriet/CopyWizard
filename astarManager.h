#pragma once
#include "singletonBase.h"
#include "tileNode.h"


//
//#define MAX_X 20
//#define MAX_Y 13
//
////������
////enum NODESTATE
////{
////	NODE_START,
////	NODE_END,
////	NODE_WALL,
////	NODE_EMPTY
////};
//
struct tagPlayer
{
	RECT rc;
	float x, y;
	float speed;
	float angle;

};
class astarManager : public singletonBase <astarManager>
{
private:

struct tagPos {
	int indexX, indexY;
};
	tileNode* _totalNode[MAXTILE_WIDTH][MAXTILE_HEIGHT];		//��ü��� 25��(������ ����)
	tileNode* _startNode;					//���۳��
	tileNode* _endNode;						//������
	tileNode* _curNode;						//������

	vector<tileNode*> _openList;			//���¸���Ʈ (ž���� ������ ��Ƶ� ����)
	vector<tileNode*> _closeList;			//���� ã�� ������ ��Ƶ� ����
	vector<tileNode*> _finalList;			//Ŭ�����Ʈ�� ����ִ� ������ ���������Ѽ� ��Ƶ� ����

	//int _count;							//���۳��, �����带 �ѹ����� �����ϱ� ���� ����
	bool _isFind;						//�� ã�ҳ�?

	RECT temp;
	RECT player;
	tagPlayer monsterMove;
	tagPlayer playerMove;

	tagPos endPos;
	tagPos startPos;


public:
	HRESULT init();
	void release();
	void update();
	void render();

	/*�߿��Լ�*/
	//��ã�� �Լ�
	void pathFinding();
	//���¸���Ʈ �߰�
	void addOpenList(int idx, int idy);
	//���¸���Ʈ ����
	void delOpenList(int index);

	//���Ǹ� ���� �Լ�
	void setNodeColor(tileNode* node, COLORREF color);

};

