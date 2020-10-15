#include "stdafx.h"
#include "unit.h"

HRESULT unit::init()
{
    tile = PLAYERDATA->_getTile();
    return S_OK;
}

void unit::addUnit(int _index, string _keyName, string type, POINT frame, float _x, float _y)
{
    zObject* zo = new zObject;
    zo->init(_index, _keyName, type, frame, _x, _y);

    if (type == "player")player = zo;

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
        vector<zObject*> tempObj;
        tempObj.swap(vUnit);
    }

    vUnit.push_back(player);

    for (int i = 0; i < wall.size(); i++)
    {
        zObject* zo = new zObject;
        zo->init(wall[i], tile[wall[i]].keyName, "wall", tile[wall[i]].frame, 0, 0);
        vUnit.push_back(zo);
    }

    for (int i = 0; i < vEnemy.size(); i++)
    {
        zObject* zo = new zObject;
       
        switch (vEnemy[i].kind)
        {
        case 0: //��
            zo->init(0, "golem", "monster", vEnemy[i].currentFrame, vEnemy[i].posX,vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 4: //�� ��Ʈ
            zo->init(4, "golemHit", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 1: //����Ʈ
            zo->init(1, "knight", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY+50);
            vUnit.push_back(zo);
            break;
        case 2: //���ӳ�
            zo->init(2, "summoner", "monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        case 3: //����
            zo->init(3, "ghoul","monster", vEnemy[i].currentFrame, vEnemy[i].posX, vEnemy[i].posY);
            vUnit.push_back(zo);
            break;
        }
       
    }
    //for(int i=0; i<)

    //ī�޶� �浹�� ���� init, push_back ���ָ� �ȴ�.
}

void unit::render(HDC hdc)
{
    this->sorting();

    for (int i = 0; i < vUnit.size(); i++)
    {
        vUnit[i]->render(hdc);
    }
}

bool compare0(zObject* i, zObject* j) {
    return (*i).getRect().top < (*j).getRect().top;
}

void unit::sorting()
{
    sort(vUnit.begin(), vUnit.end(), compare0);
}