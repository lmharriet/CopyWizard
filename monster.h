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

struct tagHit1
{
	int skillNum;
	int currentTime;
	int endTime;
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
	bool isStart;
	bool isEnd;
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


enum   ATKDIRECTION {
	MONSTER_UP,
	MONSTER_DOWN,
	MONSTER_LEFT,
	MONSTER_RIGHT,
	
	MONSTER_MAX	
};


class monster : public gameNode
{
protected:

	enum  STATEIMAGE {
		IDLE,
		WALK,
		ATK,
		HIT,
		DIE,
		

		STATEMAX
	};
	image* img = nullptr;
	image* skillImg = nullptr;
	tagTile* wall= nullptr;
	POINT frameIndexL[STATEIMAGE::STATEMAX];
	POINT frameIndexR[STATEIMAGE::STATEMAX];
	POINT currentFrame;
	POINT pos;
	POINT cul;
	STATEIMAGE state = IDLE;
	
	float speed;
	float angle;
	float hitAngle;
	float knockBack;
	MONSTERKIND kind;
	int atk;
	int atkTime;
	float armour;
	int hp;
	int hpMax;
	int count = 0;
	int delay = 0;
	int hitTime;
	int colCount = 0;
	int monsterAppearCount = 0;
	const int iceBreakTime = 100;
	int iceBreakCount = 0;
	bool isWallcol = false;
	bool isFindWayOn = false;
	bool isLeft = false;
	bool isATK = false;
	bool isDie = false;
	bool isHit = false;
	bool isDelete = false;
	bool isFxAppear = false;
	bool isRanger = false;
	bool isBulletFire = false;
	bool isMonsterApeear = false;
	bool isCardAppear = false;
	bool isCardFxAppear = false;
	bool isIceState = false;
	
	//bool isBulletEmpty = true;
	bool isAstar;
	bool isKnockBack ;
	bool atkDirection[MONSTER_MAX];
	bool bulletDirection[MONSTER_MAX];

	const float distanceMax = 500.f;
	
	unique_ptr<astarManager> astar;
	//astarManager* astar;
	
	RECT rc;
	RECT playerRC;
	RECT camRC;

	vector<tagHit1> vHit;
public:
	HRESULT init(tagTile* tile, POINT _pos); 
	void release();
	void commonUpdate();
	void render();
	virtual void update()=0;
	virtual void addRender()=0;
	virtual void stateImageRender() = 0;
	virtual void addInit()=0;

	virtual void stateIDLE() = 0;
	virtual void stateATK() = 0;
	virtual void stateDIE() = 0;
	virtual void stateHIT(POINT lPos,POINT rPos);
	


	POINT getPos() { return pos; }
	POINT getCulPos() { return cul; }
	POINT getCurrentFrame() { return currentFrame; }
	RECT getRC() { return rc; }
	MONSTERKIND getMonsterKind() { return kind; }
	bool getDelete() { return isDelete; }
	bool getATKCHECK() { return isATK; }
	bool getRange() { return isRanger; }
	bool getFx() { return isFxAppear; }
	bool getBulletFire() { return isBulletFire; }
	bool getDie() { return isDie; }
	bool getHit() { return isHit; }
	bool getAppear() { return isMonsterApeear; }
	float getAngle() { return angle; }
	int getBulletDirection() { for (int i = 0; i < MONSTER_MAX; i++) if (bulletDirection[i])return i; }
	int getHp() { return hp; }
	int getAttack() { return atk; }
	int getArmour() { return armour; }
	int getCenterY() { return pos.y + img->getFrameHeight() / 2; }
	int getCenterX() { return pos.x + img->getFrameWidth() / 2; }
	int getCulCenterX() { return cul.x + img->getFrameWidth() / 2; }
	int getCulCenterY() { return cul.y + img->getFrameHeight() / 2; }

	void setPt(float x, float y);
	void setX(int X) { pos.x = X; }
	void setY(int Y) { pos.y = Y; }
	void setPlayerRC(RECT rc) { playerRC = rc; }
	void setCamRC(RECT rc) { camRC = rc; }
	void setHp(float _atk) { hp = (int)_atk; }
	void setATK(bool atk) { isATK = atk; }
	void setFx(bool Fx) {  isFxAppear = Fx; }
	void setBulletFire(bool isFire) { isBulletFire = isFire; }
	
	void hit(int damage, float _hitAngle, float _knockBack, int skillNum, bool isCritical = false, bool isIceSTATE =false);
	bool hitCheck(int skillNum);

	void hitCul();
	bool wallCol();
	void coinDrop(int min, int max);
	void die();
};

