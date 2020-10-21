#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"
#include "bullet.h"
#include "unitNode.h"
#include "inventory.h";

#define ARCANA_SLOT 6

class player : public gameNode
{
private:
	tagTile* tile;
	vector<int> vTile;
	vector<int> vWall;

	//bullet class

	bomb* blaze;
	dragonArc* dragon;
	meteor* Meteor;
	dashFire* searingRush;
	RagingInferno* inferno;
	iceSpear* spear;


	//inventory
	inventory* inven;

private:

	tagWizardArcana arcana[ARCANA_SLOT];

	MOVE move;
	STATE state;
	

	RECT rc;

	tagCollider tileCheck[8];
	tagCollider diagonalCheck[4];

	
	float posX, posY;
	float attackAngle;
	float saveAngle ;


	int angleTenth;
	int speed;
	int gaugeTime;


	//coolTime
	int basicStateCool;
	int infernoStateCool;
	int signatureStateCool;
	int knockBackCool;
	int blazeCount;

	//skill upgrade
	float skillGauge;
	int gaugeMaxCool;
	bool upgradeReady;


	//animation count , index
	int dashCount,dashIndex;
	int atkCount, atkIndex;
	int count, index;
	int basicCount, basicIndex;
	int spearIndex;

	//animation delay
	int idleDelay;
	
	
	//damage
	int frozenTime;
	int grabbedTime;
	bool isDamaged;
	bool isGrabbed;
	
	tagKnockBack knockBack;

	//dash
	bool dashLeft, dashRight, dashUp, dashDown;
	
	//skill kind
	bool basic, standard, signature, signatureE,signatureR;



	
	// 신호 중복 방지
	bool isLeft, isRight, isUp, isDown;
	
	bool isDead;


	//temp
	bool uiOn;

	//sound
	int walkCount;

public:
	HRESULT init();
	void release();
	void update();
	void other_update();
	void render(int _index = 0);
	void invenRender();

	void bulletClassInit();

	void controller();
	void resetKey();

	////////////////////////
	////  skill setUp   ////

	void skillInit();
	void basicSetUp();
	void dashSetUp();
	void standardSetUp();
	void signatureSetUp();
	void signatureSetUpE();
	void signatureSetUpR();

	void setSkillUi(ARCANA_TYPE type, string keyName, int coolTime);
	///////////////////////



	void takeCoin();
	void takeHealball();

	//collision detection
	void tileCol();
	void colorCheck(image* img);
	void makeCol(int index, int destX, int destY, int rcSize = 7);
	void makeCol2(int index, int destX, int destY, int rcSize = 10);


	//animation
	void animation(int _index = 0);
	void frameAnimation(int frameX, int frameY, int _index = 0);
	void idleDelayCount();
	void changeState();


	//사망 판정
	void death();

	void damage(int damage, float attackAngle, float knockBackSpeed);

	void damagedCool();

	void finalAttackDamaged(int damage, int frozenCount);

	void grabbedCool();

	void chargeSkillGauge(int atkPower, int skillNum);

	void skillGaugeSetUp();

	//Don't touch
	void buttonDown();
	void viewText();


	//getter ,setter
	float getX() { return posX; }
	float getY() { return posY; }
	RECT getRect() { return rc; }
	bomb* getBlaze() { return blaze; }
	meteor* getMeteor() { return Meteor; }
	dashFire* getDashFire() { return searingRush; }
	RagingInferno* getInferno() { return inferno; }
	dragonArc* getDragon() { return dragon; }
	iceSpear* getSpear() { return spear; }

	bool getDamaged() { return isDamaged; }

	
	void setX(float x) { posX = x; }
	void setY(float y) { posY = y; }
	void reposRect() { rc = RectMakeCenter(posX, posY, 100, 100); }
	void setStateIDLE() { state = STATE::IDLE; }
	
	void setDamaged(bool temp) { isDamaged = temp; }
	
	void setIdleDelay(int value) { idleDelay = value;}

	void setTileAd(tagTile* _tile) { tile = _tile; }
	void setTileAd0(vector<int> _vTile) { vTile = _vTile; }
	void setTileAd1(vector<int> _vWall) { vWall = _vWall; }

	player() {}
	~player() {}
};