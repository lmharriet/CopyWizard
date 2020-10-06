#pragma once
#include "monster.h"



class summoner : public monster
{
private:


public:
	void addInit()override;
	void update() override;
	void render() override;

	void stateImageRender();

	void stateIDLE() override;
	void stateATK()	override;
	void stateDIE()	override;


	summoner() {}
	~summoner() {}
};