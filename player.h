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
	
	bomb* blaze;
	homingFlares* flares;
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
	int hp;

	int stateCool;
	int infernoStateCool;
	int meteorStateCool;
	int dashCount,dashIndex;
	int atkCount, atkIndex;
	int count, index;
	
	//cooltime ���� ��
	int standardCool, signatureCool;
	int standardTiemer, signatureTimer;

	//dash
	bool dashLeft, dashRight, dashUp, dashDown;
	
	//skill kind
	bool basic, standard, signature;
	
	// ��ȣ �ߺ� ����
	bool isLeft, isRight, isUp, isDown;
	
	bool isDead;
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

	void buttonDown();
	void viewText();





	//getter ,setter
	float getX() { return posX; }
	float getY() { return posY; }
	RECT getRect() { return rc; }
	bomb* getBlaze() { return blaze; }
	meteor* getMeteor() { return Meteor; }
	dashFire* getDashFire() { return searingRush; }



	void setX(float x) { posX = x; }
	void setY(float y) { posY = y; }
	void reposRect() { rc = RectMakeCenter(posX, posY, 100, 100); }
	void setStateIDLE() { state = STATE::IDLE; }

	//tile address
	void setTileAd(tagTile* _tile) { tile = _tile; }
	void setTileAd0(vector<int> v1) { vTile = v1; }
	

	player() {}
	~player() {}
};