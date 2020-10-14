#include "stdafx.h"
#include "unit.h"

void unit::addUnit(int _index, string _keyName, string type, POINT frame, float _x, float _y)
{
    zObject* zo = new zObject;
    zo->init(_index, _keyName, type, frame, _x, _y);

    if (type == "player")player = zo;

    vUnit.push_back(zo);
}

void unit::release()
{
}

void unit::update()
{
    wall = PLAYERDATA->getWall();

    vUnit.clear();
    vUnit.push_back(player);

    tagTile* tile = PLAYERDATA->_getTile();

    for (int i = 0; i < wall.size(); i++)
    {
        zObject* zo = new zObject;
        zo->init(wall[i], tile[wall[i]].keyName, "wall", tile[wall[i]].frame, 0, 0);
        vUnit.push_back(zo);
    }

    //카메라에 충돌된 적만 init, push_back 해주면 된다.
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