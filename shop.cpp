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
        AndresShop[i].Explanation = "";
        AndresShop[i].price = 0;
        AndresShop[i].pt = { 0,0 };
        AndresShop[i].frame = { 0,0 };
        AndresShop[i].isCol = false;

        if (i < 3)
        {
            NoxShop[i].isSell = false;
            NoxShop[i].keyName = "";
            NoxShop[i].Explanation = "";
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
            AndresShop[count].Explanation = item.Explanation;
            AndresShop[count].price = item.price;
            AndresShop[count].frame = item.frame;

            cout << AndresShop[count].keyName << '\n';

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
                AndresShop[count].Explanation = item.Explanation;
                AndresShop[count].price = item.price;
                AndresShop[count].frame = item.frame;

                cout << AndresShop[count].keyName << '\n';

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
            NoxShop[count].Explanation = item.Explanation;
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
                NoxShop[count].Explanation = item.Explanation;
                NoxShop[count].price = item.price;
                NoxShop[count].frame = item.frame;

                count++;
            }
        }
    }

    colItem = -1;

    addImage();

    POINT ptArr[3] = { { -2030,-1588 },{ 2062,-68 },{ 1152,2116 } };
    generate(ptArr);
    return S_OK;
}

void shop::addImage()
{
    IMAGEMANAGER->addImage("Nox", "Images/npc/blackMarketNPC.bmp", 56, 100, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("Doki", "Images/npc/richNPC.bmp", 82, 112, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("Andres", "Images/npc/shopNPC.bmp", 107, 129, true, RGB(255, 0, 255));

    IMAGEMANAGER->addImage("oldFabric", "Images/npc/oldFabric.bmp", 360, 126, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("desk", "Images/npc/desk.bmp", 390, 153, true, RGB(255, 0, 255));

    IMAGEMANAGER->addFrameImage("shopNumbers", "Images/npc/shopNumbers.bmp", 60, 10, 10, 1);
    IMAGEMANAGER->addImage("soldOut", "Images/npc/soldOut.bmp", 11, 11, true, RGB(255, 0, 255));

    IMAGEMANAGER->addImage("oldFabricShadow", "Images/npc/oldFabricShadow.bmp", 360, 126, true, RGB(255, 0, 255));

    IMAGEMANAGER->addFrameImage("itemBackBoardFrame", "Images/npc/backBoardFrame.bmp", 1125, 136, 3, 1);
    IMAGEMANAGER->addImage("ItemShadow", "Images/npc/shadow.bmp", 30, 11, true, RGB(255, 0, 255));
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

    image* img = IMAGEMANAGER->findImage("itemFrame");

    for (int i = 0; i < 3; i++)
    {
        npc[i].keyName = ranStr[i];
        npc[i].pt = arr[i];

        if (npc[i].keyName == "Nox")
        {
            EFFECT->SetCursePoint(
                { npc[i].pt.x - 90, npc[i].pt.y + 120 },
                { npc[i].pt.x +  0, npc[i].pt.y + 120 },
                { npc[i].pt.x + 90, npc[i].pt.y + 120 });

            for (int j = 0; j < 3; j++)
            {
                NoxShop[j].pt = {
                    npc[i].pt.x - 90 + (j * 90) - img->getFrameWidth() / 2,
                    npc[i].pt.y + 130 - img->getFrameHeight() / 2 };
            }
        }

        else if (npc[i].keyName == "Andres")
        {
            for (int j = 0; j < 5; j++)
            {
                AndresShop[j].pt = {
                    npc[i].pt.x - 154 + (j * 77) - img->getFrameWidth() / 2,
                    npc[i].pt.y + 85 - img->getFrameHeight() / 2 };
            }
        }

        UNITRENDER->addUnit(i, npc[i].keyName, "npc", { 0,0 }, npc[i].pt.x, npc[i].pt.y);

        cout << npc[i].keyName << " " << npc[i].pt.x << ", " << npc[i].pt.y << '\n';
    }
}

void shop::render()
{
    for (int i = 0; i < 3; i++) // NPC NUMBER
    {
        if (npc[i].keyName == "Nox")
        {
            //oldFabric.bmp
            image* ig = IMAGEMANAGER->findImage("oldFabric");
            CAMERAMANAGER->Render(getMemDC(),
                IMAGEMANAGER->findImage("oldFabric"),
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);


            //CAMERAMANAGER->Rectangle(getMemDC(), RectMakeCenter(npc[i].pt.x, npc[i].pt.y, 150, 150));

            //아이템 출력
            if (NoxShop[0].isSell == false) // 하나라도 팔리면 다 없어짐
            {
                EFFECT->cursePointActive();
                //shadow
                image* shadow = IMAGEMANAGER->findImage("oldFabricShadow");

                CAMERAMANAGER->AlphaRender(getMemDC(), shadow,
                    npc[i].pt.x - ig->getWidth() / 2,
                    npc[i].pt.y - ig->getHeight() / 2 + 150
                , 140);

                EFFECT->curseRenderBack(getMemDC());
                for (int j = 0; j < 3; j++)
                {
                    image* img = IMAGEMANAGER->findImage("itemFrame");

                    CAMERAMANAGER->FrameRender(getMemDC(),
                        img,
                        NoxShop[j].pt.x,
                        NoxShop[j].pt.y,
                        NoxShop[j].frame.x,
                        NoxShop[j].frame.y);

                    NoxShop[j].rc = RectMake(NoxShop[j].pt.x - img->getFrameWidth() / 2,
                        NoxShop[j].pt.y - img->getFrameHeight() / 2,
                        70, 70);
                }
                EFFECT->curseRenderFront(getMemDC());
            }
        }
        else if (npc[i].keyName == "Andres")
        {
            image* ig = IMAGEMANAGER->findImage("desk");
            CAMERAMANAGER->Render(getMemDC(),
                ig,
                npc[i].pt.x - ig->getWidth() / 2,
                npc[i].pt.y - ig->getHeight() / 2 + 150);

            //아이템 출력
            for (int j = 0; j < 5; j++) // ITEM
            {
                image* numImg = IMAGEMANAGER->findImage("shopNumbers");

                //가격표 출력
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

                    else // price가 3자리 수 일 때
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

                else // 아이템이 팔렸으면?
                {
                    image* soldOutImg = IMAGEMANAGER->findImage("soldOut");
                    CAMERAMANAGER->Render(getMemDC(), soldOutImg,
                        npc[i].pt.x - 158 + (j * 78) + 4,
                        AndresShop[j].pt.y + 73);

                    continue;
                }

                //그림자 출력
                image* shadow = IMAGEMANAGER->findImage("ItemShadow");
                CAMERAMANAGER->Render(getMemDC(), shadow,
                    AndresShop[j].pt.x + 4,
                    AndresShop[j].pt.y + 40);

                image* img = IMAGEMANAGER->findImage("itemFrame");

                //아이템 출력
                CAMERAMANAGER->FrameRender(getMemDC(),
                    img,
                    AndresShop[j].pt.x,
                    AndresShop[j].pt.y,
                    AndresShop[j].frame.x, 
                    AndresShop[j].frame.y);

                AndresShop[j].rc = RectMake(AndresShop[j].pt.x - img->getFrameWidth()/2,
                    AndresShop[j].pt.y - img->getFrameHeight()/2,
                    70, 70);
            }
        }
    }
}

void shop::colRender()
{
    //HFONT myFont = CreateFont(25, 0, 0, 0, 1000, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "font/MunroSmall.ttf");
    //HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);

    image* back = IMAGEMANAGER->findImage("itemBackBoardFrame");
    RECT rt;
    int frameX = 0;
    for (int j = 0; j < 5; j++)
    {
        if (!AndresShop[j].isCol || AndresShop[j].isSell) continue;

        //정보 출력
        if (AndresShop[j].isCol && !AndresShop[j].isSell)
        {
            rt = RectMakeCenter(
                CAMERAMANAGER->GetRelativeX(AndresShop[j].pt.x) + 20,
                CAMERAMANAGER->GetRelativeY(AndresShop[j].pt.y),
                280, 70);

            //==========================================================
            if (AndresShop[j].Explanation.size() > 69) frameX = 2;

            else if (AndresShop[j].Explanation.size() > 34) frameX = 1;

            CAMERAMANAGER->AlphaFrameRender(getMemDC(), back,
                AndresShop[j].pt.x + 20 - back->getFrameWidth() / 2,
                AndresShop[j].pt.y - 10 - back->getFrameHeight() / 2,
                frameX, 0, 130);
            //==========================================================

            //SetTextColor(getMemDC(), RGB(255, 255, 255));

            TextOut(getMemDC(), CAMERAMANAGER->GetRelativeX(AndresShop[j].pt.x) - 30,
                 CAMERAMANAGER->GetRelativeY(AndresShop[j].pt.y) - 70,
                AndresShop[j].keyName.c_str(), AndresShop[j].keyName.length());

            const char* ch = AndresShop[j].Explanation.c_str();

            DrawText(getMemDC(), ch, -1, &rt, DT_CENTER | DT_WORDBREAK);
        }
    }

    if (NoxShop->isSell == false)
    {
        for (int j = 0; j < 3; j++)
        {
            if (NoxShop[j].isCol == false)continue;

            rt = RectMakeCenter(
                CAMERAMANAGER->GetRelativeX(NoxShop[j].pt.x) + 20,
                CAMERAMANAGER->GetRelativeY(NoxShop[j].pt.y) - 40,
                320, 70);

            //==========================================================
            if (NoxShop[j].Explanation.size() > 69) frameX = 2;

            else if (NoxShop[j].Explanation.size() > 34) frameX = 1;

            CAMERAMANAGER->AlphaFrameRender(getMemDC(), back,
                NoxShop[j].pt.x + 20 - back->getFrameWidth() / 2,
                NoxShop[j].pt.y - 50 - back->getFrameHeight() / 2,
                frameX, 0, 130);
            //==========================================================

            //SetTextColor(getMemDC(), RGB(255, 255, 255));

            TextOut(getMemDC(), CAMERAMANAGER->GetRelativeX(NoxShop[j].pt.x) - 30,
                CAMERAMANAGER->GetRelativeY(NoxShop[j].pt.y) - 110,
                NoxShop[j].keyName.c_str(), NoxShop[j].keyName.length());

            const char* ch = NoxShop[j].Explanation.c_str();

            DrawText(getMemDC(), ch, -1, &rt, DT_CENTER | DT_WORDBREAK);
        }
    }

    //SelectObject(getMemDC(), oldFont);
    //DeleteObject(myFont);
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
                cout << AndresShop[colItem].keyName << "을 구매!" << '\n';

                AndresShop[colItem].isSell = true;
                AndresShop[colItem].frame = { 0,0 };

                //1. 여기에 player stat을 조정
                PLAYERDATA->setStat(AndresShop[colItem].keyName);

                //2. vector<string> vInven에 push_Back

                tagItem _item = ITEM->getItem(AndresShop[colItem].keyName);
                PLAYERDATA->pushInven(_item);

                //emotion play
                EFFECT->setEmotionEffect("buyEmote_Andres", { AndresShop[2].pt.x + 20 , AndresShop[2].pt.y - 130 });
                //3. 아이템의 가격만큼 코인을 잃음
                PLAYERDATA->setCoin(PLAYERDATA->getCoin() - AndresShop[colItem].price);
            }
            else
            {
                cout << "잔액이 부족합니다!" << '\n';
            }
        }

        else
        {
            //buy item
            cout << NoxShop[colItem - 5].keyName << "을 구매!" << '\n';

            for (int i = 0; i < 3; i++)
            {
                NoxShop[i].isSell = true;
                NoxShop[i].frame = { 0,0 };
            }

            //1. 여기에 player stat을 조정
            PLAYERDATA->setStat(NoxShop[colItem - 5].keyName);

            //2. vector<string> vInven에 push_Back
            tagItem _item = ITEM->getItem(NoxShop[colItem - 5].keyName);
            PLAYERDATA->pushInven(_item);

            //emotion play
            EFFECT->setEmotionEffect("buyEmote_Nox", { NoxShop[1].pt.x + 20 , NoxShop[1].pt.y - 200 });
        }
    }
}