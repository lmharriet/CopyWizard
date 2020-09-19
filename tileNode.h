#pragma once
#include "gameNode.h"

#define MAXUNDOSIZE 5
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
struct tagMap
{
	RECT rc;

	POINT frame;

	bool active;
	bool isCol;
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