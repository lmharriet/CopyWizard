#pragma once
///MAP SIZE///

//최대 가로 길이//
#define MAXTILE_WIDTH 150
//최대 세로 길이//
#define MAXTILE_HEIGHT 150
//모든 타일의 개수//
#define MAXTILE (MAXTILE_WIDTH * MAXTILE_HEIGHT)

#define TILESIZE 32

enum class OPTION
{
	WALL_MENU,
	TILE_MENU,
	OBJECT_MENU,

	SELECT_MENU
};

enum class DIRECTION
{
	LEFT_TOP,
	TOP,
	RIGHT_TOP,
	LEFT,
	RIGHT,
	LEFT_DOWN,
	BOTTOM,
	RIGHT_DOWN
};

struct tagRC
{
	RECT rc;
	bool isCol;
};

enum class TERRAIN
{
	WALL,
	TILE,
	OBJECT,
	IMG,
	DECO,

	NONE
};
struct tagTile
{
	RECT rc;
	TERRAIN kind;
	string keyName;

	POINT frame;
};


enum class TOOL
{
	DRAW,
	ERASE,
	SPOID,

	NONE
};
struct tagUser
{
	TERRAIN kind;
	string KeyName;

	int transY;
	int delay;
};
struct tagDrag
{
	RECT rc;
	POINT start, end;
};


struct tagCam
{
	RECT rc;
	POINT pt;
};
