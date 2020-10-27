#include "stdafx.h"
#include "unit.h"

HRESULT unit::init()
{
    tile = PLAYERDATA->_getTile();

    return S_OK;
}

void unit::addUnit(int _index, string _keyName, string type, POINT frame, float _x, float _y)
{
    shared_ptr<zObject> zo = make_shared<zObject>();

    zo->init(_index, _keyName, type, frame, _x, _y);

    if (type == "player")player = zo;

    if (type == "npc") npc[_index] = zo;

    if (type == "chest") chest = zo;

    vUnit.push_back(zo);
}




void unit::enemyInit(int kind, POINT currentFrame, POINT pos)
{
    tagEnemy enemy;
    enemy.kind = kind;
    enemy.currentFrame = currentFrame;
    enemy.posX = (float)pos.x;
    enemy.posY = (float)pos.y;
    vEnemy.push_back(enemy);
}

void unit::release()
{
    SAFE_DELETE(tile);
}

void unit::update()
{
    wall = PLAYERDATA->getWall();

    int size = vUnit.size();
    for (int i = 0; i < size; i++)
    {
        vector<shared_ptr<zObject>> tempObj;
        tempObj.swap(vUnit);
    }

    vUnit.push_back(player);

    for(int i = 0; i < 2; i++) vUnit.push_back(npc[i]);

    vUnit.push_back(chest);

    size = wall.size();
    for (int i = 0; i < size; i++)
    {
        shared_ptr<zObject> zo = make_shared<zObject>();

        zo->init(wall[i], tile[wall[i]].keyName, "wall", tile[wall[i]].frame, 0, 0);
        vUnit.push_back(zo);
    }

    size = vEnemy.size();
    for (int i = 0; i < size; i++)
    {
        shared_ptr<zObject> zo = make_shared<zObject>();
       
        switch (vEnemy[i].kind)
        {
        case 0: //골렘
            zo->init(0, "golem", "monster", vEnemy[i].currentFrame, vEnemy[i].posX,vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 4: //골렘 히트
            zo->init(4, "golemHit", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 1: //나이트
            zo->init(1, "knight", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY+50);
            vUnit.push_back(zo);
            break;
        case 2: //서머너
            zo->init(2, "summoner", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 3: //구울
            zo->init(3, "ghoul","monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 5: // 구울라지
            zo->init(5, "ghoulLarge", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 6: //구울라지 히트
            zo->init(6, "ghoulLargeHit", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        }
       
    }
}

void unit::render(HDC hdc)
{
    this->sorting();

    int size = vUnit.size();
    for (int i = 0; i < size; i++)
    {
        vUnit[i]->render(hdc);
    }
}

bool compare0(shared_ptr<zObject> i, shared_ptr<zObject> j) {
    return (*i).getRect().top < (*j).getRect().top;
}

void unit::sorting()
{
    sort(vUnit.begin(), vUnit.end(), compare0);
}