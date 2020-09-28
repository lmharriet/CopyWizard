#pragma once
#include "gameNode.h"

#define MAX_X 5
#define MAX_Y 5

//노드상태
enum NODESTATE
{
	NODE_START,
	NODE_END,
	NODE_WALL,
	NODE_EMPTY
};

struct tagPlayer
{
	float x, y;
	float speed;
	float angle;

};

//노드 클래스
class node
{
public:
	RECT rc;				//렉트
	int idx, idy;			//인덱스
	int F, G, H;			//F = G + H, G => 시작 to 현재, H = 현재 to 종료
	node* parentNode;		//부모를 가리킬 노드(이전 노드)
	NODESTATE nodeState;	//노드상태 (시작, 종료, 벽, 빈노드)

	//노드 생성자
	node() {}
	//노드 생성자 (생성자 오버로딩)
	node(int _idx, int _idy)
	{
		idx = _idx;
		idy = _idy;
		F = G = H = 0;
		nodeState = NODE_EMPTY;
		parentNode = NULL;
	}
};

struct tagPos {
	int indexX, indexY;
};
class astarTestScene : public gameNode
{
private:
	node* _totalNode[MAX_X][MAX_Y];		//전체노드 25개(보드판 역할)
	node* _startNode;					//시작노드
	node* _endNode;						//종료노드
	node* _curNode;						//현재노드

	vector<node*> _openList;			//오픈리스트 (탑색할 노드들을 담아둘 벡터)
	vector<node*> _closeList;			//길을 찾은 노드들을 담아둘 벡터
	vector<node*> _finalList;			//클로즈리스트에 담겨있는 노드들을 리버스시켜서 담아둘 벡터

	int _count;							//시작노드, 종료노드를 한번씩만 선택하기 위한 변수
	bool _isFind;						//길 찾았냐?


	RECT player;
	RECT monster;
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
	void setNodeColor(node* node, COLORREF color);

};

