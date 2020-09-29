#pragma once
#include "singletonBase.h"
#include "tileNode.h"


//
//#define MAX_X 20
//#define MAX_Y 13
//
////노드상태
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
	tileNode* _totalNode[MAXTILE_WIDTH][MAXTILE_HEIGHT];		//전체노드 25개(보드판 역할)
	tileNode* _startNode;					//시작노드
	tileNode* _endNode;						//종료노드
	tileNode* _curNode;						//현재노드

	vector<tileNode*> _openList;			//오픈리스트 (탑색할 노드들을 담아둘 벡터)
	vector<tileNode*> _closeList;			//길을 찾은 노드들을 담아둘 벡터
	vector<tileNode*> _finalList;			//클로즈리스트에 담겨있는 노드들을 리버스시켜서 담아둘 벡터

	//int _count;							//시작노드, 종료노드를 한번씩만 선택하기 위한 변수
	bool _isFind;						//길 찾았냐?

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

	/*중요함수*/
	//길찾기 함수
	void pathFinding();
	//오픈리스트 추가
	void addOpenList(int idx, int idy);
	//오픈리스트 삭제
	void delOpenList(int index);

	//편의를 위한 함수
	void setNodeColor(tileNode* node, COLORREF color);

};

