#pragma once
#include "gameNode.h"
#include "tileNode.h"



struct tagPlayer  //���߿� ���� ��
{
	RECT rc;
	float x, y;
	float speed;
	float angle;

};
class astarManager
{
private:

	tileNode* totalNode[MAXTILE_WIDTH][MAXTILE_HEIGHT];		//��ü��� 25��(������ ����)
	tileNode* startNode;					//���۳��
	tileNode* endNode;						//������
	tileNode* curNode;						//������
	tagTile* tile;							//���� ��Ÿ��
	vector<tileNode*> openList;			//���¸���Ʈ (ž���� ������ ��Ƶ� ����)
	vector<tileNode*> closeList;			//���� ã�� ������ ��Ƶ� ����
	vector<tileNode*> finalList;			//Ŭ�����Ʈ�� ����ִ� ������ ���������Ѽ� ��Ƶ� ����

	//int _count;							//���۳��, �����带 �ѹ����� �����ϱ� ���� ����
	bool isFind;						//�� ã�ҳ�?

	RECT temp;
	tagPlayer monsterMove;
	tagPlayer playerMove;

	POINT endPos;
	POINT startPos;

	//��Ʈ Ȯ�ο� ķ
	RECT cam;

public:
	HRESULT init(tagTile* _tile);
	void release();
	void update();
	void render(HDC hdc);

	/*�߿��Լ�*/
	//��ã�� �Լ�
	void pathFinding();
	//���¸���Ʈ �߰�
	void addOpenList(int idx, int idy);
	//���¸���Ʈ ����
	void delOpenList(int index);

	//���Ǹ� ���� �Լ�
	void setNodeColor(tileNode* node, COLORREF color, HDC hdc);
	//void setTileNode(tagTile* _tile) { tile = _tile; }
	void setCam(RECT _cam) { cam = _cam; }
	void setEndRC(RECT rc) { playerMove.rc = rc; }
	void setStartRC(RECT rc) { monsterMove.rc = rc; }
	//void setStartPos(POINT pos) { startPos = pos; }
	//void setEndPos(POINT pos) { endPos = pos; }
};







class monster : public gameNode
{
protected:

	image* img;
	RECT rc;
	POINT pos;
	POINT cul;
	float speed;
	float angle;
	int atk;
	int atkTime;
	int armour;
	int hp;
	int hpMax;
	bool isFindWayOn;
	astarManager* astar;
	RECT playerRC;
	RECT camRC;
	 
	

public:
	virtual HRESULT init(tagTile* tile)=0;
	virtual void release()=0;
	virtual void update()=0;
	virtual void render()=0;

	inline virtual POINT getPos() { return pos; }
	inline virtual RECT getRC() { return rc; }
	inline virtual void setPlayerRC(RECT rc) { playerRC = rc; }
	inline virtual void setCamRC(RECT rc) { camRC = rc; }
};

