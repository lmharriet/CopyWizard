#include "stdafx.h"
#include "shop.h"

HRESULT shop::init()
{
    for (int i = 0; i < 3; i++)
    {
        npc[i].keyName = "";
        npc[i].pt = { 0,0 };
    }
    addImage();
    return S_OK;
}

void shop::addImage()
{
    IMAGEMANAGER->addImage("blackMarketNPC", "Images/npc/blackMarketNPC.bmp", 56, 100, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("richNPC", "Images/npc/richNPC.bmp", 82, 112, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("shopNPC", "Images/npc/shopNPC.bmp", 84, 129, true, RGB(255, 0, 255));

    IMAGEMANAGER->addImage("oldFabric", "Images/npc/oldFabric.bmp", 120*3, 42*3, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("desk", "Images/npc/desk.bmp", 120 * 3, 42 * 3, true, RGB(255, 0, 255));
}

void shop::generate(POINT arr[3])
{
    string ranStr[3] = { "blackMarketNPC","richNPC","shopNPC" };
    for (int i = 0; i < 100; i++)
    {
        string temp;
        temp = ranStr[i % 3];
        int ran = RANDOM->range(0, 2);
        ranStr[i % 3] = ranStr[ran];
        ranStr[ran] = temp;
    }

    for (int i = 0; i < 3; i++)
    {
        npc[i].keyName = ranStr[i];
        npc[i].pt = arr[i];
    }
}

void shop::render()
{
    for (int i = 0; i < 3; i++)
    {
        image* img = IMAGEMANAGER->findImage(npc[i].keyName);
        CAMERAMANAGER->Render(getMemDC(), img,
            npc[i].pt.x - img->getWidth() / 2,
            npc[i].pt.y - img->getHeight() / 2);

        if (npc[i].keyName == "blackMarketNPC")
        {
            //oldFabric.bmp
            image* ig = IMAGEMANAGER->findImage("oldFabric");
            CAMERAMANAGER->Render(getMemDC(),
                IMAGEMANAGER->findImage("oldFabric"),
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);
        }
        else if (npc[i].keyName == "shopNPC")
        {
            image* ig = IMAGEMANAGER->findImage("desk");
            CAMERAMANAGER->Render(getMemDC(),
                IMAGEMANAGER->findImage("desk"),
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);
        }
    }
}