#include "stdafx.h"
#include "unit.h"

HRESULT unit::init()
{
    tile = PLAYERDATA->_getTile();

    for (int i = 0; i < 500; i++)
    {
        tempObject.push_back(new zObject);
    }

    return S_OK;
}

zObject* unit::getObject()
{
    zObject* object;

    if (tempObject.empty())
    {
        object = new zObject;
    }

    else
    {
        object = tempObject[0];
        tempObject.erase(tempObject.begin());
    }

    return object;
}

void unit::rtnObject(zObject* _object)
{
    tempObject.push_back(_object);
}

void unit::addUnit(int _index, string _keyName, string type, POINT frame, float _x, float _y)
{
    zObject* zo = getObject();

    zo->init(_index, _keyName, type, frame, _x, _y);

    if (type == "player")player = zo;

    if (type == "npc") npc[_index] = *zo;

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
        rtnObject(vUnit[i]);

        vector<zObject*> tempObj;
        tempObj.swap(vUnit);
    }

    vUnit.push_back(player);

    for(int i = 0; i < 3; i++) vUnit.push_back(&npc[i]);

    for (int i = 0; i < wall.size(); i++)
    {
        zObject* zo = getObject();

        zo->init(wall[i], tile[wall[i]].keyName, "wall", tile[wall[i]].frame, 0, 0);
        vUnit.push_back(zo);
    }

    for (int i = 0; i < vEnemy.size(); i++)
    {
        zObject* zo = getObject();
       
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