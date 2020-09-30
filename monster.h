#pragma once
#include "gameNode.h"
#include "tileNode.h"



struct tagPlayer  //나중에 버릴 것
{
	RECT rc;
	float x, y;
	float speed;
	float angle;

};
class astarManager
{
private:

	tileNode* totalNode[MAXTILE_WIDTH][MAXTILE_HEIGHT];		//전체노드 25개(보드판 역할)
	tileNode* startNode;					//시작노드
	tileNode* endNode;						//종료노드
	tileNode* curNode;						//현재노드
	tagTile* tile;							//현재 맵타일
	vector<tileNode*> openList;			//오픈리스트 (탑색할 노드들을 담아둘 벡터)
	vector<tileNode*> closeList;			//길을 찾은 노드들을 담아둘 벡터
	vector<tileNode*> finalList;			//클로즈리스트에 담겨있는 노드들을 리버스시켜서 담아둘 벡터

	//int _count;							//시작노드, 종료노드를 한번씩만 선택하기 위한 변수
	bool isFind;						//길 찾았냐?

	RECT temp;
	tagPlayer monsterMove;
	tagPlayer playerMove;

	POINT endPos;
	POINT startPos;

	//렉트 확인용 캠
	RECT cam;

public:
	HRESULT init(tagTile* _tile);
	void release();
	void update();
	void render(HDC hdc);

	/*중요함수*/
	//길찾기 함수
	void pathFinding();
	//오픈리스트 추가
	void addOpenList(int idx, int idy);
	//오픈리스트 삭제
	void delOpenList(int index);

	//편의를 위한 함수
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

