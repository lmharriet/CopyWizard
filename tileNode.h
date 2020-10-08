#pragma once
///MAP SIZE///

//�ִ� ���� ����//
#define MAXTILE_WIDTH 150
//�ִ� ���� ����//
#define MAXTILE_HEIGHT 150
//��� Ÿ���� ����//
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

	//���̽�Ÿ ����� ���� ���������
	int idx, idy;			//�ε���
	int F, G, H;			//F = G + H, G => ���� to ����, H = ���� to ����
	tagTile* parentNode;		//�θ� ����ų ���(���� ���)
	NODESTATE nodeState;	//������ (����, ����, ��, ����)

	//��� ������
	tagTile() {}
	//��� ������ (������ �����ε�)
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

enum class MONSTERKIND {
	GOLEM,
	KNIGHT,
	SUMMONER,
};

struct tagSpawn
{
	POINT pt;
};