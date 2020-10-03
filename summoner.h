#pragma once
#include "monster.h"



class summoner : public monster
{
private:

	bool isDetect;
	int time;

public:
	void addInit()override;
	void render() override;
	void update() override;

	void skill();


	//충돌용 렉트 가져오기
	int getX() { return pos.x; }
	int getY() { return pos.y; }

	bool getFind() { return isDetect; }
	void setFind(bool tmp) { isDetect = tmp; }
	bool checkAttack() { return isATK; }
	bool setAttack(bool tmp) { return isATK = tmp; }


	summoner() {}
	~summoner() {}
};