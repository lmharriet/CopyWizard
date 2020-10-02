#pragma once
#include "gameNode.h"

//부모클래스 => 이놈을 상속받아서 보스, 일반몬스터를 만든다
class enemy : public gameNode
{
protected:
	image* _image;		//에너미 이미지
	float posX, posY;	//위치
	RECT _rc;			//충돌용 렉트

	RECT area;			//감지용 렉트
	bool isDetect, isAttack;

	//프레임 이미지를 돌리기 위한 변수들
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	//랜덤으로 총알발사 쿨타임 주기
	int _fireCount;
	int _rndFireCount;


	int time;
public:
	HRESULT init(const char* imageName, POINT position);
	void release();
	void update();
	void render();

	virtual void move();			//업데이트
	virtual void draw();			//렌더
	virtual void animation();		//업데이트

	bool bulletCountFire();

	//충돌용 렉트 가져오기
	RECT getRect() { return _rc; }
	RECT getArea() { return area; }
	float getX() { return posX; }
	float getY() { return posY; }

	bool getFind() { return isDetect; }
	void setFind(bool tmp) { isDetect = tmp; }

	bool checkAttack() { return isAttack; }
	bool setAttack(bool tmp) { return isAttack = tmp; }

	enemy() {}
	~enemy() {}
};