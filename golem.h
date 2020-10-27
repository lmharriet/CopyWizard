#pragma once
#include "monster.h"

class golem : public monster
{
private:
	
	POINT smallSlashIndex;
	image* hitImg;

	bool isAtkImgCount;
	int fxCount;
	

public:
	void addInit()override;
	void update() override;
	void addRender() override;
	

	void stateImageRender() override;


	void stateIDLE() override;
	void stateATK()	override;
	void stateDIE()	override;
	void stateWalk();


};

