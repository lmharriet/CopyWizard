#pragma once
///MAP SIZE///

//최대 가로 길이//
#define MAXTILE_WIDTH 100
//최대 세로 길이//
#define MAXTILE_HEIGHT 100
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
struct tagIcon
{
	RECT rc;
	bool isCol;
};

enum class TERRAIN
{
	WALL,
	TILE,
	OBJECT
};
struct tagTile
{
	RECT rc;
	TERRAIN kind;
};








struct tagCam
{
	RECT rc;
	POINT pt;
};