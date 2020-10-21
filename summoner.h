#pragma once
#include "monster.h"

class summoner : public monster
{
private:
	bool isCasting;
	bool isCharging;
	float imgSize;
	int randomTime;
	int fireCount;
public:
	void addInit()override;
	void update() override;
	void addRender() override;
	void stateImageRender() override;

	void stateIDLE() override;
	void stateATK()	override;
	void stateDIE()	override;

	

};