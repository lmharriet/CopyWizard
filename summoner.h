#pragma once
#include "monster.h"



class summoner : public monster
{
private:


public:
	void addInit()override;
	void render() override;
	void update() override;

	void stateImageRender();
	


	summoner() {}
	~summoner() {}
};