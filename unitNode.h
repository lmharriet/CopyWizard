#pragma once

struct tagCollider
{
	RECT rc;
	POINT pos;
	bool isCol;
};
struct tagKnockBack
{
	float angle;
	float speed;
	float percent;
};


//Dash,RUN
enum class STATE
{
	IDLE, RUN, DASH, DAMAGED, DIE,
	BASIC,
	STANDARD,
	SIGNATURE
};

enum class MOVE
{
	LEFT,
	LEFT_TOP,
	RIGHT,
	RIGHT_TOP,
	UP,
	DOWN,
	LEFT_DOWN,
	RIGHT_DOWN,
};

enum ARCANA_NAME
{
	BLAZE, SEARING_DASH, INFERNO,METEOR, DRAGONARC
};