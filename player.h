#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"
#include "bullet.h"
#include "unitNode.h"
#include "inventory.h";

class player : public gameNode
{
private:
	tagTile* tile;
	vector<int> vTile;
	vector<int> vWall;
	
	bomb* blaze;
	cleaver* flares;
	meteor* Meteor;
	dashFire* searingRush;
	RagingInferno* inferno;

	inventory* inven;
private:

	MOVE move;
	STATE state;

	RECT rc;
	tagCollider tileCheck[8];
	tagCollider diagonalCheck[4];

	
	float posX, posY;
	float attackAngle;
	float saveAngle ;
	float saveAngle2 ;


	int angleTenth;
	int speed;
	int gaugeTime;


	int stateCool;
	int infernoStateCool;
	int meteorStateCool;
	
	int blazeCount;
	
	int dashCount,dashIndex;
	int atkCount, atkIndex;
	int count, index;

	
	
	//damage
	float damageAngle;
	int damageAngleTenth;
	int frozenTime;
	bool isDamaged;
	
	//dash
	bool dashLeft, dashRight, dashUp, dashDown;
	
	//skill kind
	bool basic, standard, signature;
	
	// ��ȣ �ߺ� ����
	bool isLeft, isRight, isUp, isDown;
	
	bool isDead;

	//sound
	int walkCount;

public:
	HRESULT init();
	void release();
	void update();
	void other_update();
	void render();
	void invenRender();

	void bulletClassInit();

	void controller();
	void dashFunction();
	void resetKey();
	void blazeSetUp();
	void standardSetUp();
	void signatureSetUp();

	void takeCoin();
	void takeHealball();
	//collision detection
	void tileCol();
	void colorCheck(image* img);
	void makeCol(int index, int destX, int destY, int rcSize = 7);

	void makeCol2(int index, int destX, int destY, int rcSize = 10);


	//animation
	void animation();
	void frameAnimation(int frameX, int frameY);
	void changeState();


	//��� ����
	void death();

	void damage(int damage, float attackAngle);

	void damagedCool();

	void buttonDown();
	void viewText();


	//getter ,setter
	float getX() { return posX; }
	float getY() { return posY; }
	float getDamageAngle() { return damageAngle; }
	int getDamageAngleTenth() { return damageAngleTenth; }
	RECT getRect() { return rc; }
	bomb* getBlaze() { return blaze; }
	meteor* getMeteor() { return Meteor; }
	dashFire* getDashFire() { return searingRush; }
	RagingInferno* getInferno() { return inferno; }

	bool getDamaged() { return isDamaged; }


	void setX(float x) { posX = x; }
	void setY(float y) { posY = y; }
	void setDamagedAngle(float *attackAngle) { damageAngle = *attackAngle; }
	void setDamagedAngleTenth(int angleTenth) { damageAngleTenth = angleTenth; }
	void reposRect() { rc = RectMakeCenter(posX, posY, 100, 100); }
	void setStateIDLE() { state = STATE::IDLE; }
	void setDamaged(bool temp) { isDamaged = temp; }
	
	void setTileAd(tagTile* _tile) { tile = _tile; }
	void setTileAd0(vector<int> _vTile) { vTile = _vTile; }
	void setTileAd1(vector<int> _vWall) { vWall = _vWall; }

	player() {}
	~player() {}
};