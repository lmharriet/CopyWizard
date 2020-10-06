#pragma once

struct tagCollider
{
	RECT rc;
	POINT pos;
	bool isCol;
};
//Dash,RUN
enum class STATE
{
	IDLE, RUN, DASH,DIE,
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
