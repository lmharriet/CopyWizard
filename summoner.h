#pragma once
#include "enemy.h"



class summoner : public enemy
{
private:


public:
	void animation() override;
	void draw() override;
	void move() override;

	void skill();

	summoner() {}
	~summoner() {}
};