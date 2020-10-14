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
        AndresShop[i].isCol = false;

        if (i < 3)
        {
            NoxShop[i].isSell = false;
            NoxShop[i].keyName = "";
            NoxShop[i].price = 0;
            NoxShop[i].pt = { 0,0 };
            NoxShop[i].frame = { 0,0 };
            NoxShop[i].isCol = false;
        }
    }

    //random item
    int count = 0;
    while (count < 5)
    {
        tagItem item = ITEM->getRandomItem(true);

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

    //random item
    count = 0;
    while (count < 3)
    {
        tagItem item = ITEM->getRandomItem(false);

        if (count == 0)
        {
            NoxShop[count].keyName = item.keyName;
            NoxShop[count].price = item.price;
            NoxShop[count].frame = item.frame;

            count++;
        }
        else
        {
            bool check = false;

            for (int i = 0; i < count; i++)
            {
                if (NoxShop[i].keyName == item.keyName)
                {
                    check = true;
                    break;
                }
            }

            if (check == false)
            {
                NoxShop[count].keyName = item.keyName;
                NoxShop[count].price = item.price;
                NoxShop[count].frame = item.frame;

                count++;
            }
        }
    }

    colItem = -1;

    addImage();
    return S_OK;
}

void shop::addImage()
{
    IMAGEMANAGER->addImage("Nox", "Images/npc/blackMarketNPC.bmp", 56, 100, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("Doki", "Images/npc/richNPC.bmp", 82, 112, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("Andres", "Images/npc/shopNPC.bmp", 84, 129, true, RGB(255, 0, 255));

    IMAGEMANAGER->addImage("oldFabric", "Images/npc/oldFabric.bmp", 120*3, 42*3, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("desk", "Images/npc/desk.bmp", 390, 153, true, RGB(255, 0, 255));

    IMAGEMANAGER->addFrameImage("shopNumbers", "Images/npc/shopNumbers.bmp", 60, 10, 10, 1);
    IMAGEMANAGER->addImage("soldOut", "Images/npc/soldOut.bmp", 11, 11, true, RGB(255, 0, 255));
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
    for (int i = 0; i < 3; i++) // NPC NUMBER
    {
        image* img = IMAGEMANAGER->findImage(npc[i].keyName);
        CAMERAMANAGER->Render(getMemDC(), img,
            npc[i].pt.x - img->getWidth() / 2,
            npc[i].pt.y - img->getHeight() / 2);

        //CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(npc[i].pt.x, npc[i].pt.y, 50, 50));

        if (npc[i].keyName == "Nox")
        {
            //oldFabric.bmp
            image* ig = IMAGEMANAGER->findImage("oldFabric");
            CAMERAMANAGER->Render(getMemDC(),
                IMAGEMANAGER->findImage("oldFabric"),
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);

            //������ ���
            if (NoxShop[0].isSell == false) // �ϳ��� �ȸ��� �� ������
            {
                for (int j = 0; j < 3; j++)
                {
                    image* img = IMAGEMANAGER->findImage("itemFrame");

                    NoxShop[j].pt = {
                        npc[i].pt.x - 90 + (j * 90) - img->getFrameWidth() / 2,
                        npc[i].pt.y + 140 - img->getFrameHeight() / 2 };


                    CAMERAMANAGER->FrameRender(getMemDC(),
                        img,
                        NoxShop[j].pt.x,
                        NoxShop[j].pt.y,
                        NoxShop[j].frame.x,
                        NoxShop[j].frame.y);

                    NoxShop[j].rc = RectMake(NoxShop[j].pt.x - img->getFrameWidth() / 2,
                        NoxShop[j].pt.y - img->getFrameHeight() / 2,
                        70, 70);

                    //CAMERAMANAGER->Rectangle(getMemDC(), NoxShop[j].rc);
                }
            }
        }
        else if (npc[i].keyName == "Andres")
        {
            image* ig = IMAGEMANAGER->findImage("desk");
            CAMERAMANAGER->Render(getMemDC(),
                ig,
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);

            //������ ���
            for (int j = 0; j < 5; j++) // ITEM
            {
                image* img = IMAGEMANAGER->findImage("itemFrame");

                AndresShop[j].pt = {
                    npc[i].pt.x - 154 + (j * 77) - img->getFrameWidth() / 2,
                    npc[i].pt.y + 85 - img->getFrameHeight() / 2 };


                CAMERAMANAGER->FrameRender(getMemDC(),
                    img,
                    AndresShop[j].pt.x,
                    AndresShop[j].pt.y,
                    AndresShop[j].frame.x, 
                    AndresShop[j].frame.y);

                AndresShop[j].rc = RectMake(AndresShop[j].pt.x - img->getFrameWidth()/2,
                    AndresShop[j].pt.y - img->getFrameHeight()/2,
                    70, 70);

                //CAMERAMANAGER->Rectangle(getMemDC(), AndresShop[j].rc);

                image* numImg = IMAGEMANAGER->findImage("shopNumbers");

                //����ǥ ���
                
                if (AndresShop[j].isSell == false)
                {
                    if (AndresShop[j].price < 100)
                    {
                        int one = AndresShop[j].price % 10;
                        int ten = AndresShop[j].price / 10;

                        CAMERAMANAGER->FrameRender(getMemDC(), numImg,
                            npc[i].pt.x - 158 + (j * 78) + 2,
                            AndresShop[j].pt.y + 73,
                            ten, 0);
                        CAMERAMANAGER->FrameRender(getMemDC(), numImg,
                            npc[i].pt.x - 158 + (j * 78) + 7 + 2,
                            AndresShop[j].pt.y + 73,
                            one, 0);
                    }

                    else // price�� 3�ڸ� �� �� ��
                    {
                        int one = AndresShop[j].price % 10;
                        int ten = AndresShop[j].price / 10 % 10;
                        int hun = AndresShop[j].price / 100;

                        CAMERAMANAGER->FrameRender(getMemDC(), numImg,
                            npc[i].pt.x - 158 + (j * 78),
                            AndresShop[j].pt.y + 73,
                            hun, 0);
                        CAMERAMANAGER->FrameRender(getMemDC(), numImg,
                            npc[i].pt.x - 158 + (j * 78) + 7,
                            AndresShop[j].pt.y + 73,
                            ten, 0);
                        CAMERAMANAGER->FrameRender(getMemDC(), numImg,
                            npc[i].pt.x - 158 + (j * 78) + 14,
                            AndresShop[j].pt.y + 73,
                            one, 0);
                    }
                }
                
                else // �������� �ȷ�����?
                {
                    image* soldOutImg = IMAGEMANAGER->findImage("soldOut");
                    CAMERAMANAGER->Render(getMemDC(), soldOutImg, 
                        npc[i].pt.x - 158 + (j * 78) + 4,
                        AndresShop[j].pt.y + 73);
                }
            }
        }
    }
}

void shop::shopCollider(RECT rc)
{
    for (int i = 0; i < 5; i++)
    {
        AndresShop[i].isCol = false;

        if (i < 3)
        {
            NoxShop[i].isCol = false;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (AndresShop[i].isSell == false)
        {
            if (colCheck(rc, AndresShop[i].rc))
            {
                AndresShop[i].isCol = true;
                colItem = i;

                return;
            }
        }

        if (i < 3)
        {
            if (NoxShop[i].isSell == false)
            {
                if (colCheck(rc, NoxShop[i].rc))
                {
                    NoxShop[i].isCol = true;
                    colItem = i + 5;

                    return;
                }
            }
        }
    }

    colItem = -1;
}

void shop::buyItem()
{
    if (colItem != -1 && INPUT->GetKeyDown(0x46))
    {
        if (colItem < 5)
        {
            if (PLAYERDATA->getCoin() >= AndresShop[colItem].price)
            {
                //buy item
                cout << AndresShop[colItem].keyName << "�� ����!" << '\n';

                AndresShop[colItem].isSell = true;
                AndresShop[colItem].frame = { 0,0 };

                //1. ���⿡ player stat�� ����
                PLAYERDATA->setStat(AndresShop[colItem].keyName);

                //2. vector<string> vInven�� push_Back
                PLAYERDATA->pushInven(AndresShop[colItem].keyName);

                //3. �������� ���ݸ�ŭ ������ ����
                PLAYERDATA->setCoin(PLAYERDATA->getCoin() - AndresShop[colItem].price);
            }
            else cout << "�ܾ��� �����մϴ�!" << '\n';
        }

        else
        {
            //buy item
            cout << NoxShop[colItem - 5].keyName << "�� ����!" << '\n';

            for (int i = 0; i < 3; i++)
            {
                NoxShop[i].isSell = true;
                NoxShop[i].frame = { 0,0 };
            }

            //1. ���⿡ player stat�� ����
            PLAYERDATA->setStat(NoxShop[colItem - 5].keyName);

            //2. vector<string> vInven�� push_Back
            PLAYERDATA->pushInven(NoxShop[colItem - 5].keyName);
        }
    }
}