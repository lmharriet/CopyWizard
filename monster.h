#pragma once
#include "gameNode.h"


//#define MAX_X 20
//#define MAX_Y 13

//������
//enum NODESTATE
//{
//	NODE_START,
//	NODE_END,
//	NODE_WALL,
//	NODE_EMPTY
//};
//
//struct tagPlayer
//{
//	RECT rc;
//	float x, y;
//	float speed;
//	float angle;
//
//};
//
////��� Ŭ����
//class node
//{
//public:
//	RECT rc;				//��Ʈ
//	int idx, idy;			//�ε���
//	int F, G, H;			//F = G + H, G => ���� to ����, H = ���� to ����
//	node* parentNode;		//�θ� ����ų ���(���� ���)
//	NODESTATE nodeState;	//������ (����, ����, ��, ����)
//
//	//��� ������
//	node() {}
//	//��� ������ (������ �����ε�)
//	node(int _idx, int _idy)
//	{
//		idx = _idx;
//		idy = _idy;
//		F = G = H = 0;
//		nodeState = NODE_EMPTY;
//		parentNode = NULL;
//	}
//};





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

	

public:
	virtual HRESULT init()=0;
	virtual void release()=0;
	virtual void update()=0;
	virtual void render()=0;

	inline virtual POINT getPos() { return pos; }
	inline virtual RECT getRC() { return rc; }
};

