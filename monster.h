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
	int monsterPosX;
	int monsterPosY;

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
	void update(RECT _camRC, RECT _monsterRC, RECT _playerRC, float* angle);
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
	
	tileNode* getFirstTile() { if (!finalList.empty())return finalList[0]; else return NULL; }
	
};

enum  STATEIMAGE {
	IDLE,
	WALK,
	ATK,
	HIT,
	DIE,

	STATEMAX
};


enum  ATKDIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	
	MAX	
};

class monster : public gameNode
{
protected:

	image* img;
	image* skillImg;
	POINT frameIndexL[STATEIMAGE::STATEMAX];
	POINT frameIndexR[STATEIMAGE::STATEMAX];
	POINT pos;
	POINT cul;
	STATEIMAGE state = STATEIMAGE::IDLE;
	
	float speed;
	float angle;
	int atk;
	int atkTime;
	int armour;
	int hp;
	int hpMax;
	int count = 0;
	int delay = 0;
	bool isFindWayOn = false;
	bool isLeft = false;
	bool isATK = false;
	bool isDie = false;
	bool isDelete = false;
	bool isFxAppear = false;
	bool isAstar;
	bool isKnockBack ;
	const float distanceMax = 700.f;
	
	astarManager* astar;
	
	RECT rc;
	RECT playerRC;
	RECT camRC;

	 
	

public:
	HRESULT init(tagTile* tile, const char* fileName, POINT _pos, float _speed, int _hp,  const char* skillImgName, bool _isKnockBack = true);
	void release();
	virtual void update()=0;
	virtual void render()=0;
	virtual void addInit()=0;

	inline POINT getPos() { return pos; }
	inline RECT getRC() { return rc; }
	inline bool getDelete() { return isDelete; }

	inline void setPlayerRC(RECT rc) { playerRC = rc; }
	inline void setCamRC(RECT rc) { camRC = rc; }
	inline void setHp(float _atk) { hp = _atk; }
	inline int getHp() { return hp; }
	
	void hit(int damage, float hitAngle, float knockBack);
	void coinDrop(int min, int max);
	void die();
};

