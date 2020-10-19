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

    for (int i = 0; i < vUnit.size(); i++)
    {
        vector<shared_ptr<zObject>> tempObj;
        tempObj.swap(vUnit);
    }

    vUnit.push_back(player);

    for(int i = 0; i < 3; i++) vUnit.push_back(npc[i]);

    for (int i = 0; i < wall.size(); i++)
    {
        shared_ptr<zObject> zo = make_shared<zObject>();

        zo->init(wall[i], tile[wall[i]].keyName, "wall", tile[wall[i]].frame, 0, 0);
        vUnit.push_back(zo);
    }

    for (int i = 0; i < vEnemy.size(); i++)
    {
        shared_ptr<zObject> zo = make_shared<zObject>();
       
        switch (vEnemy[i].kind)
        {
        case 0: //°ñ·½
            zo->init(0, "golem", "monster", vEnemy[i].currentFrame, vEnemy[i].posX,vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 4: //°ñ·½ È÷Æ®
            zo->init(4, "golemHit", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 1: //³ªÀÌÆ®
            zo->init(1, "knight", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY+50);
            vUnit.push_back(zo);
            break;
        case 2: //¼­¸Ó³Ê
            zo->init(2, "summoner", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 3: //±¸¿ï
            zo->init(3, "ghoul","monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        }
       
    }
}

void unit::render(HDC hdc)
{
    this->sorting();

    for (int i = 0; i < vUnit.size(); i++)
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