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
	SIGNATURE,
	DRAGON,
	SPEAR
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

enum class ARCANA_TYPE
{
	TYPE_EMPTY, TYPE_BASIC, TYPE_DASH, TYPE_STANDARD, TYPE_SIGNATURE
};

struct tagWizardArcana
{
	ARCANA_TYPE type;
	string skillName;
	string explanation;
	int coolTime;
};