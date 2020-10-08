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
	MONSTER_MENU,
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
enum class NODESTATE
{
	NODE_START,
	NODE_END,
	NODE_WALL,
	NODE_EMPTY
};

typedef struct tagTile
{
	RECT rc;
	TERRAIN kind;
	string keyName;

	POINT frame;

	//에이스타 사용을 위한 멤버변수들
	int idx, idy;			//인덱스
	int F, G, H;			//F = G + H, G => 시작 to 현재, H = 현재 to 종료
	tagTile* parentNode;		//부모를 가리킬 노드(이전 노드)
	NODESTATE nodeState;	//노드상태 (시작, 종료, 벽, 빈노드)

	//노드 생성자
	tagTile() {}
	//노드 생성자 (생성자 오버로딩)
	tagTile(int _idx, int _idy)
	{
		idx = _idx;
		idy = _idy;
		F = G = H = 0;
		nodeState = NODESTATE::NODE_EMPTY;
		parentNode = NULL;
	}

}tileNode;

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



struct tagSpawn
{
	POINT pt;
};