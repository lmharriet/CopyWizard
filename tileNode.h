#pragma once
#include "gameNode.h"

///MAP SIZE///

//최대 가로 길이//
#define MAXTILE_WIDTH 100
//최대 세로 길이//
#define MAXTILE_HEIGHT 100
//모든 타일의 개수//
#define MAXTILE (MAXTILE_WIDTH * MAXTILE_HEIGHT)

#define TILESIZE 48
//COLOR
#define BLACK	 RGB(   0,   0,   0 )
#define RED		 RGB( 255,   0,   0 )
#define GREEN	 RGB(   0, 255,   0 )
#define BLUE	 RGB(   0,   0, 255 )

#define RIGHT_TILE	{ 2, 1 }
#define LEFT_TILE	{ 0, 1 }
#define TOP_TILE	{ 1, 0 }
#define BOTTOM_TILE { 1, 2 }
#define CENTER_TILE { 1, 1 }

enum STATE
{
	NONE,
	DRAW,	  // 단일
	DRAW_SET, // 세트
	ERASE,
	MOVE
};
enum TILEKIND
{
	WAY,
	WALL,
	OBJECT,
};
struct tagMap
{
	RECT rc;

	POINT frame;

	bool active;//활성화 되었냐?
	bool isCol; //드래그에 포함되었냐?

	bool isCam; //카메라 안에 들어와 있냐?
};
struct tagSample
{
	RECT rc;

	POINT frame;
};
struct tagMouse
{
	RECT rc;
	POINT start, end;

	bool active; //draw,erase mode
	bool dragMode; //drag?single?
	POINT frame;
	STATE saveBrush;
};