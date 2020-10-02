#pragma once

struct tagCollider
{
	RECT rc;
	POINT pos;
	bool isCol;
};
enum class STATE
{
	IDLE, RUN, DASH,
	BASIC,
	STANDARD,
	SIGNATURE
};
//Dash,RUN
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
