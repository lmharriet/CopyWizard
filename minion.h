#pragma once
#include "enemy.h"

class minion : public enemy
{
public:
	minion() {}
	~minion() {}
};

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