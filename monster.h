#pragma once
#include "gameNode.h"


//#define MAX_X 20
//#define MAX_Y 13

//노드상태
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
////노드 클래스
//class node
//{
//public:
//	RECT rc;				//렉트
//	int idx, idy;			//인덱스
//	int F, G, H;			//F = G + H, G => 시작 to 현재, H = 현재 to 종료
//	node* parentNode;		//부모를 가리킬 노드(이전 노드)
//	NODESTATE nodeState;	//노드상태 (시작, 종료, 벽, 빈노드)
//
//	//노드 생성자
//	node() {}
//	//노드 생성자 (생성자 오버로딩)
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

