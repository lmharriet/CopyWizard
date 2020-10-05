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
	int monsterPosX;
	int monsterPosY;

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
	void update(RECT _camRC, RECT _monsterRC, RECT _playerRC, float* angle);
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

	virtual void stateIDLE() = 0;
	virtual void stateATK() = 0;
	virtual void stateDIE() = 0;
	


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

