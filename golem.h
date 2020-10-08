#pragma once
#include "monster.h"

class golem : public monster
{
private:
	
	POINT smallSlashIndex;
	image* hitImg;

public:
	void addInit()override;
	void update() override;
	void render() override;

	void stateImageRender();


	void stateIDLE() override;
	void stateATK()	override;
	void stateDIE()	override;
	void stateWalk();


};

