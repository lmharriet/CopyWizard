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
	bomb* blaze;
	homingFlares* flares;
	meteor* Meteor;
	inventory* inven;
private:

	MOVE move;
	STATE state;

	RECT rc;
	tagCollider tileCheck[8];
	tagCollider diagonalCheck[4];

//	test
	tagDash test;
	
	float posX, posY;
	//float angle;
	float attackAngle;
	float saveAngle ;

	int angleTenth;
	int speed;

	int stateCool;
	int meteorCool;
	int dashCount,dashIndex;
	int atkCount, atkIndex;
	int count, index;
	
	bool dashLeft, dashRight, dashUp, dashDown;
	bool basic, standard, signature;
	
	// ��ȣ �ߺ� ����
	bool isLeft, isRight, isUp, isDown;
	

public:
	HRESULT init();
	void release();
	void update();
	void other_update();
	void render();

	void controller();
	void dashFunction();
	void resetKey();
	void blazeSetUp();
	void standardSetUp();
	void signatureSetUp();


	//collision detection
	void tileCol();
	void colorCheck(image* img);
	void makeCol(int index, int destX, int destY, int rcSize = 7);

	void makeCol2(int index, int destX, int destY, int rcSize = 10);


	//animation
	void animation();
	void frameAnimation(int frameX, int frameY);
	void changeState();

	void buttonDown();
	void viewText();


	//getter ,setter
	float getX() { return posX; }
	float getY() { return posY; }
	RECT getRect() { return rc; }
	bomb* getBlaze() { return blaze; }
	meteor* getMeteor() { return Meteor; }

	void setX(float x) { posX = x; }
	void setY(float y) { posY = y; }
	void reposRect() { rc = RectMakeCenter(posX, posY, 100, 100); }
	void setStateIDLE() { state = STATE::IDLE; }

	//tile address
	void setTileAd(tagTile* _tile) { tile = _tile; }

	

	player() {}
	~player() {}
};