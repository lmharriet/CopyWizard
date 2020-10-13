#include "stdafx.h"
#include "shop.h"

HRESULT shop::init()
{
    for (int i = 0; i < 3; i++)
    {
        npc[i].keyName = "";
        npc[i].pt = { 0,0 };
    }

    for (int i = 0; i < 5; i++)
    {
        AndresShop[i].isSell = false;
        AndresShop[i].keyName = "";
        AndresShop[i].price = 0;
        AndresShop[i].pt = { 0,0 };
        AndresShop[i].frame = { 0,0 };
    }

    //random item
    int count = 0;
    while (count < 5)
    {
        tagItem item = ITEM->getRandomItem();

        if (count == 0)
        {
            AndresShop[count].keyName = item.keyName;
            AndresShop[count].price = item.price;
            AndresShop[count].frame = item.frame;

            count++;
        }
        else
        {
            bool check = false;

            for (int i = 0; i < count; i++)
            {
                if (AndresShop[i].keyName == item.keyName)
                {
                    check = true;
                    break;
                }
            }

            if (check == false)
            {
                AndresShop[count].keyName = item.keyName;
                AndresShop[count].price = item.price;
                AndresShop[count].frame = item.frame;

                count++;
            }
        }
    }

    addImage();
    return S_OK;
}

void shop::addImage()
{
    IMAGEMANAGER->addImage("Nox", "Images/npc/blackMarketNPC.bmp", 56, 100, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("Doki", "Images/npc/richNPC.bmp", 82, 112, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("Andres", "Images/npc/shopNPC.bmp", 84, 129, true, RGB(255, 0, 255));

    IMAGEMANAGER->addImage("oldFabric", "Images/npc/oldFabric.bmp", 120*3, 42*3, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("desk", "Images/npc/desk.bmp", 120 * 3, 42 * 3, true, RGB(255, 0, 255));

    IMAGEMANAGER->addFrameImage("shopNumbers", "Images/npc/shopNumbers.bmp", 30, 5, 10, 1);
}

void shop::generate(POINT arr[3])
{
    string ranStr[3] = { "Nox","Doki","Andres" };
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
        cout << npc[i].keyName << " " << npc[i].pt.x << ", " << npc[i].pt.y << '\n';
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

        CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(npc[i].pt.x, npc[i].pt.y, 50, 50));

        if (npc[i].keyName == "Nox")
        {
            //oldFabric.bmp
            image* ig = IMAGEMANAGER->findImage("oldFabric");
            CAMERAMANAGER->Render(getMemDC(),
                IMAGEMANAGER->findImage("oldFabric"),
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);
        }
        else if (npc[i].keyName == "Andres")
        {
            image* ig = IMAGEMANAGER->findImage("desk");
            CAMERAMANAGER->Render(getMemDC(),
                ig,
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);

            AndresShop[i].pt = npc[i].pt;

            //아이템 출력
            for (int j = 0; j < 5; j++)
            {
                image* img = IMAGEMANAGER->findImage("itemFrame");

                AndresShop[j].pt = {
                    npc[i].pt.x - 140 + (j * 70) - img->getFrameWidth() / 2,
                    npc[i].pt.y + 95 - img->getFrameHeight() / 2 };


                CAMERAMANAGER->FrameRender(getMemDC(),
                    img,
                    AndresShop[j].pt.x,
                    AndresShop[j].pt.y,
                    AndresShop[j].frame.x, 
                    AndresShop[j].frame.y);

                //image* numImg = IMAGEMANAGER->findImage("shopNumbers");
                image* numImg = IMAGEMANAGER->findImage("numbers");
                //가격표 출력
                if (AndresShop[j].price < 100)
                {
                    int one = AndresShop[j].price % 10;
                    int ten = AndresShop[j].price / 10;
                }

                else // price가 3자리 수 일 때
                {
                    int one = AndresShop[j].price % 10;
                    int ten = AndresShop[j].price / 10 % 10;
                    int hun = AndresShop[j].price / 100;

                    CAMERAMANAGER->FrameRender(getMemDC(), numImg, AndresShop[j].pt.x - 25, AndresShop[j].pt.y, hun, 0);
                    CAMERAMANAGER->FrameRender(getMemDC(), numImg, AndresShop[j].pt.x - 00, AndresShop[j].pt.y, ten, 0);
                    CAMERAMANAGER->FrameRender(getMemDC(), numImg, AndresShop[j].pt.x + 25, AndresShop[j].pt.y, one, 0);
                }
            }
        }
    }
}