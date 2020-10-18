#include "stdafx.h"
#include "effectManager.h"

HRESULT effectManager::init()
{
    addImage();
    time = 0;
    aTime = 0;

    int arr[8] = { 255,255,255,204,153,102,51,25 };
    for (int i = 0; i < 8; i++)pEft.opacity[i] = arr[i];
    pEft.frameX = 0;
    pEft.maxFrame = IMAGEMANAGER->findImage("portalEffect")->getMaxFrameX();
    pEft.isActive = false;
    pEft.pos = { 0,0 };

    return S_OK;
}

void effectManager::pRender(HDC hdc)
{
    time++;

    if (vDashEft.empty())return;

    for (int i = 0; i < vDashEft.size();)
    {
        POINT position = { CAMERAMANAGER->GetRelativeX(vDashEft[i].pos.x),CAMERAMANAGER->GetRelativeY(vDashEft[i].pos.y) };

        IMAGEMANAGER->findImage(vDashEft[i].keyName)->frameRender(hdc, position.x, position.y, vDashEft[i].imgCount, vDashEft[i].frameY);

        if (!vDashEft[i].flipImg)
        {
            if (time % 5 == 0)vDashEft[i].imgCount++;

            if (vDashEft[i].imgCount == vDashEft[i].maxFrame)
            {
                vDashEft.erase(vDashEft.begin() + i);
            }
            else i++;
        }
        else
        {
            if (time % 5 == 0)vDashEft[i].imgCount--;

            if (vDashEft[i].imgCount == 0)
            {
                vDashEft.erase(vDashEft.begin() + i);
            }
            else i++;
        }
    }
}

void effectManager::render(HDC hdc)
{
    //vector<tagEffect> vEft;
    ////데미지 이펙트 관리
    //vector<tagDamageEffect> dEft;
    ////포탈 이펙트 
    //tagPortalEffect pEft;
    //cout << dEft.size() << '\n';


    if (vEft.empty()) return;

    for (int i = 0; i < vEft.size();)
    {
        image* img = IMAGEMANAGER->findImage(vEft[i].keyName);

        if (vEft[i].isFrame) // 프레임 이미지인지?
        {
            if (vEft[i].isEraseSize)
            {
                CAMERAMANAGER->StretchFrameRender(hdc, img,
                    vEft[i].pos.x - img->getFrameWidth() * vEft[i].currentSize / 2,
                    vEft[i].pos.y - img->getFrameHeight() * vEft[i].currentSize / 2,
                    vEft[i].imgCount, 0, vEft[i].currentSize);
            }

            else
            {
                CAMERAMANAGER->FrameRender(hdc, img,
                    vEft[i].pos.x - img->getFrameWidth() / 2,
                    vEft[i].pos.y - img->getFrameHeight() / 2,
                    vEft[i].imgCount, 0);
            }

            if (vEft[i].flipImg == false) // 애니메이션 정방향 출력
            {
                if (time % vEft[i].frameDelay == 0 && vEft[i].imgCount < vEft[i].maxFrame) vEft[i].imgCount++;

                if (vEft[i].isEraseSize) // 삭제조건 : 사이즈
                {
                    vEft[i].currentSize += vEft[i].increaseSize;

                    float cul = abs(vEft[i].currentSize - vEft[i].endSize);

                    if (cul < 0.01)vEft.erase(vEft.begin() + i);
                    else i++;
                }

                else if (vEft[i].isEraseTime == false) // 삭제조건 : 프레임
                {
                    if (vEft[i].imgCount == vEft[i].maxFrame) vEft.erase(vEft.begin() + i);
                    else i++;
                }

                else // 삭제조건 : 시간
                {
                    if (vEft[i].currentTime == vEft[i].eraseTime) vEft.erase(vEft.begin() + i);
                    else
                    {
                        vEft[i].currentTime++;

                        i++;
                    }
                }
            }

            else // 애니메이션 역방향 출력
            {
                if (time % vEft[i].frameDelay == 0 && vEft[i].imgCount > 0) vEft[i].imgCount--;

                if (vEft[i].isEraseSize) // 삭제조건 : 사이즈
                {
                    float cul = abs(vEft[i].currentSize - vEft[i].endSize);

                    if (cul < 0.01)vEft.erase(vEft.begin() + i);
                    else i++;
                }

                else if (vEft[i].isEraseTime == false) // 삭제조건 : 프레임
                {
                    if (vEft[i].imgCount == 0) vEft.erase(vEft.begin() + i);
                    else i++;
                }

                else // 삭제조건 : 시간
                {
                    if (vEft[i].currentTime == vEft[i].eraseTime) vEft.erase(vEft.begin() + i);
                    else
                    {
                        vEft[i].currentTime++;

                        i++;
                    }
                }
            }

        }

        else // 프레임 이미지가 아님 (size로 삭제)
        {
            if (vEft[i].isEraseSize)
            {
                //출력
                CAMERAMANAGER->StretchRender(hdc, img,
                    vEft[i].pos.x - (img->getWidth() * vEft[i].currentSize / 2),
                    vEft[i].pos.y - (img->getHeight() * vEft[i].currentSize / 2),
                    vEft[i].currentSize);

                //사이즈 변경
                vEft[i].currentSize += vEft[i].increaseSize;

                //삭제
                float cul = abs(vEft[i].currentSize - vEft[i].endSize);

                if (cul < 0.01) vEft.erase(vEft.begin() + i);
                else i++;
            }
            
            else // time del
            {
                //출력
                CAMERAMANAGER->AlphaRender(hdc, img, 
                    vEft[i].pos.x - (img->getWidth() / 2),
                    vEft[i].pos.y - (img->getHeight() / 2),
                    vEft[i].opacity);
                
                //이동
                if (time % 2 == 0) vEft[i].pos.y--;

                //투명도 변경
                if(vEft[i].opacity > 0)vEft[i].opacity--;

                //삭제
                if (vEft[i].opacity == 0) vEft.erase(vEft.begin() + i);

                else i++;
            }
        }
    }
}

void effectManager::portalRender(HDC hdc)
{
    if (pEft.isActive == false)return;

    image* img = IMAGEMANAGER->findImage("portalEffect");
    //렌더
    CAMERAMANAGER->AlphaFrameRender(hdc, img,
        pEft.pos.x - img->getFrameWidth() / 2,
        pEft.pos.y - img->getFrameHeight() + 70,
        pEft.frameX, 0, pEft.opacity[pEft.frameX]);

    //비활성화
    if (pEft.frameX == pEft.maxFrame)
    {
        lightEffect(pEft.pos, 20);
        pEft.isActive = false;
    }
    else
    {
        //프레임 증가
        if (time % 4 == 0) pEft.frameX++;
    }
}

void effectManager::dRender(HDC hdc)
{
    if (dEft.empty())return;

    for (int i = 0; i < dEft.size();)
    {
        image* img = IMAGEMANAGER->findImage("damageEffect");
        CAMERAMANAGER->FrameRender(hdc, img, 
            dEft[i].pos.x - img->getFrameWidth()/2,
            dEft[i].pos.y - img->getFrameHeight()/2,
            dEft[i].frameX, dEft[i].frameY);

        if (time % 5 == 0) dEft[i].frameX++;

        if (dEft[i].frameX == dEft[i].maxFrame)
        {
            dEft.erase(dEft.begin() + i);
        }
        else i++;
    }
}

void effectManager::addImage()
{
    //dash
    IMAGEMANAGER->addFrameImage("dashVertical", "Images/effect/dashVertical.bmp", 385, 44, 5, 2);
    IMAGEMANAGER->addFrameImage("dashHorizon", "Images/effect/dashHorizon.bmp", 112, 154, 5, 2);

    IMAGEMANAGER->addFrameImage("dashLURU", "Images/effect/dashLURU.bmp", 395, 158, 5, 2);
    IMAGEMANAGER->addFrameImage("dashLDRD", "Images/effect/dashLDRD.bmp", 395, 158, 5, 2);

    //other
    IMAGEMANAGER->addFrameImage("portalEffect", "Images/effect/portal.bmp", 384 * 2, 256 * 2, 8, 1);

    IMAGEMANAGER->addImage("lightEffect1", "Images/effect/notFrame/light1.bmp", 5, 5, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("lightEffect2", "Images/effect/notFrame/light2.bmp", 5, 5, true, RGB(255, 0, 255));
    IMAGEMANAGER->addImage("lightEffect3", "Images/effect/notFrame/light3.bmp", 5, 5, true, RGB(255, 0, 255));

    IMAGEMANAGER->addImage("noFrameStone", "Images/effect/notFrame/stone.bmp", 50, 56, true, RGB(255, 0, 255));

    //card
    IMAGEMANAGER->addFrameImage("flipCard_small", "Images/effect/monster/flipCard0.bmp", 1088, 96, 17, 1);
    IMAGEMANAGER->addFrameImage("flipCard_big", "Images/effect/monster/flipCard2.bmp", 2176, 192, 17, 1);

    //damage
    IMAGEMANAGER->addFrameImage("damageEffect", "Images/effect/monster/damageEffect.bmp", 300, 300, 4, 4);

    //curse
    IMAGEMANAGER->addFrameImage("curseEffect", "Images/effect/curseItem.bmp", 112, 248, 4, 4);

    //emotion
    IMAGEMANAGER->addFrameImage("buyEmote_Andres", "Images/effect/npc/buyEmote1.bmp", 352, 56, 8, 1);
    IMAGEMANAGER->addFrameImage("buyEmote_Nox", "Images/effect/npc/buyEmote2.bmp", 440, 56, 10, 1);

    //ult
    IMAGEMANAGER->addFrameImage("ultLight", "Images/effect/player/ultLight.bmp", 6202, 721, 7, 1);

    //portal
    IMAGEMANAGER->addFrameImage("portal0", "Images/effect/purple_portal.bmp", 488, 110, 8, 1);
    IMAGEMANAGER->addFrameImage("portal1", "Images/effect/green_portal.bmp", 488, 110, 8, 1);
    IMAGEMANAGER->addFrameImage("portal2", "Images/effect/red_portal.bmp", 488, 110, 8, 1);

    IMAGEMANAGER->addFrameImage("portalFrame", "Images/effect/portalFrame.bmp", 177, 108, 3, 1);
}

void effectManager::dashEffect(MOVE direction, POINT pos)
{
    switch (direction)
    {
    case MOVE::LEFT:
        setDash("dashHorizon", 0, false, { pos.x,pos.y - 25 });
        break;
    case MOVE::LEFT_TOP:
        setDash("dashLURU", 0, false, { pos.x,pos.y });
        break;
    case MOVE::RIGHT:
        setDash("dashHorizon", 1, true, { pos.x,pos.y - 25 });
        break;
    case MOVE::RIGHT_TOP:
        setDash("dashLURU", 1, true, { pos.x - 35,pos.y - 35 });
        break;
    case MOVE::UP:
        setDash("dashVertical", 0, false, { pos.x - 35,pos.y });
        break;
    case MOVE::DOWN:
        setDash("dashVertical", 1, false, { pos.x - 35,pos.y });
        break;
    case MOVE::LEFT_DOWN:
        setDash("dashLDRD", 0, false, { pos.x - 35,pos.y - 35 });
        break;
    case MOVE::RIGHT_DOWN:
        setDash("dashLDRD", 1, true, { pos.x,pos.y });
        break;
    default:
        return;
    }
}

void effectManager::setDash(string keyName, int frameY, bool flip, POINT pt)
{
    tagDashEffect effect;

    effect.keyName = keyName;
    effect.frameY = frameY;
    effect.flipImg = flip;
    effect.maxFrame = IMAGEMANAGER->findImage(keyName)->getMaxFrameX();
    
    if (!flip) effect.imgCount = 0;
    else effect.imgCount = effect.maxFrame;

    effect.pos = pt;

    vDashEft.push_back(effect);
}

//이펙트를 실행 시키기 전 addImage(frame)가 되어있는지 확인해야한다.
void effectManager::setEffect(string keyName, POINT pt, bool flip, bool isEraseTime, int eraseTime, int frameDelay)
{
    tagEffect effect;
    effect.keyName = keyName;
    effect.pos = pt;
    effect.isFrame = true;
    effect.flipImg = flip;
    effect.isEraseTime = isEraseTime;
    effect.currentTime = 0;
    effect.eraseTime = eraseTime;
    effect.maxFrame = IMAGEMANAGER->findImage(keyName)->getMaxFrameX();
    effect.frameDelay = frameDelay;

    effect.isEraseSize = false;
    effect.currentSize = 1.f;

    if (!flip) effect.imgCount = 0;
    else effect.imgCount = effect.maxFrame;

    vEft.push_back(effect);
}

void effectManager::setEffect(string keyName, POINT pt, int minOpacity, int maxOpacity)
{
    tagEffect effect;
    effect.keyName = keyName;
    effect.pos = pt;
    effect.isFrame = false;
    effect.isEraseTime = true;
    effect.eraseTime = 0;

    effect.opacity = RANDOM->range(minOpacity, maxOpacity);

    effect.flipImg = false;
    effect.currentTime = 0;
    effect.maxFrame = 0;
    effect.frameDelay = 0;
    effect.isEraseSize = false;
    effect.currentSize = 1.f;

    vEft.push_back(effect);
}

void effectManager::setPortalEffect(POINT pt)
{
    pEft.pos = pt;
    pEft.frameX = 0;
    pEft.isActive = true;
}

void effectManager::setEffect(string keyName, POINT pt, bool isFrameImg, int frameDelay, bool flip, float increaseSize, float startSize, float endSize)
{
    tagEffect effect;
    effect.keyName = keyName;
    effect.pos = pt;
    effect.isFrame = isFrameImg;
    effect.flipImg = flip;
    effect.isEraseTime = false;
    effect.currentTime = 0;
    effect.eraseTime = 0;

    effect.isEraseSize = true;
    effect.increaseSize = increaseSize;
    effect.currentSize = startSize;
    effect.endSize = endSize;

    if (effect.isFrame)
    {
        effect.maxFrame = IMAGEMANAGER->findImage(keyName)->getMaxFrameX();
        effect.frameDelay = frameDelay;
        if (!flip) effect.imgCount = 0;
        else effect.imgCount = effect.maxFrame;
    }

    vEft.push_back(effect);
}

void effectManager::setBackEffect(string keyName, POINT pt, int frameDelay)
{
    tagEffect effect;
    effect.keyName = keyName;
    effect.pos = pt;

    effect.imgCount = 0;
    effect.maxFrame = IMAGEMANAGER->findImage(keyName)->getMaxFrameX();
    effect.frameDelay = frameDelay;

    vBackEft.push_back(effect);
}

void effectManager::backEftRender(HDC hdc)
{
    for (int i = 0; i < vBackEft.size();)
    {
        //렌더
        image* img = IMAGEMANAGER->findImage(vBackEft[i].keyName);
        CAMERAMANAGER->FrameRender(hdc, img,
            vBackEft[i].pos.x - img->getFrameWidth()/2,
            vBackEft[i].pos.y - img->getFrameHeight()/2,
            vBackEft[i].imgCount,
            0);

        //증가
        if (vBackEft[i].imgCount == 3 || vBackEft[i].imgCount == 4)
        {
            if (time % (vBackEft[i].frameDelay * 4) == 0)vBackEft[i].imgCount++;
        }
        
        else if (time % vBackEft[i].frameDelay == 0)vBackEft[i].imgCount++;

        //삭제
        if (vBackEft[i].imgCount == vBackEft[i].maxFrame)vBackEft.erase(vBackEft.begin() + i);
        else i++;
    }
}

void effectManager::damageEffect(POINT pt)
{
    tagDamageEffect effect;
    effect.frameX = 0;
    effect.frameY = RANDOM->range(0, IMAGEMANAGER->findImage("damageEffect")->getMaxFrameY());
    effect.maxFrame = IMAGEMANAGER->findImage("damageEffect")->getMaxFrameX();
    effect.pos = pt;

    dEft.push_back(effect);
}

void effectManager::lightEffect(POINT pt, int maxEffect)
{
    for (int i = 0; i < maxEffect; i++)
    {
        int ranX = RANDOM->range(-50,50);
        int ranY = RANDOM->range(-300,50);
        int ranTime = RANDOM->range(50, 100);

        string str = "lightEffect";
        int ranStr = RANDOM->range(1, 3);
        char ch[10];
        str += itoa(ranStr, ch, 10);

        setEffect(str, { pt.x + ranX,pt.y + ranY }, 35, 75);
    }
}

void effectManager::SetCursePoint(POINT _pt0, POINT _pt1, POINT _pt2)
{
    cursePoint.createDelay = 60;
    cursePoint.isActive = true;

    cursePoint.pt[0] = _pt0;
    cursePoint.pt[1] = _pt1;
    cursePoint.pt[2] = _pt2;

    cursePoint.renderIndex = 0;
    cursePoint.curTime = 0;
}

void effectManager::cursePointActive()
{
    cursePoint.curTime++;

    if (cursePoint.curTime == 30)
    {
        //생성1
        image* img = IMAGEMANAGER->findImage("curseEffect");

        for (int i = 0; i < 3; i++)
        {
            tagCurseEffect eft;
            eft.curTime = RANDOM->range(0, 12);
            eft.frame = { 0,RANDOM->range(0,img->getMaxFrameY()) };
            eft.maxFrame = img->getMaxFrameX();

            eft.pos = { cursePoint.pt[i].x + RANDOM->range(-15,15), cursePoint.pt[i].y + RANDOM->range(-8,8) };
            curseBackEft.push_back(eft);
        }
    }

    if (cursePoint.curTime == cursePoint.createDelay)
    {
        cursePoint.curTime = 0;
        
        //생성2
        image* img = IMAGEMANAGER->findImage("curseEffect");

        for (int i = 0; i < 3; i++)
        {
            tagCurseEffect eft;
            eft.curTime = RANDOM->range(0, 12);
            eft.frame = { 0,RANDOM->range(0,img->getMaxFrameY()) };
            eft.maxFrame = img->getMaxFrameX();

            eft.pos = { cursePoint.pt[i].x + RANDOM->range(-12,12),cursePoint.pt[i].y + RANDOM->range(-6,6) };
            curseFrontEft.push_back(eft);
        }
    }
}

void effectManager::curseRenderBack(HDC hdc)
{
    for (int i = 0; i < curseBackEft.size();)
    {
        //렌더
        CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage("curseEffect"),
            curseBackEft[i].pos.x - IMAGEMANAGER->findImage("curseEffect")->getFrameWidth() / 2,
            curseBackEft[i].pos.y - IMAGEMANAGER->findImage("curseEffect")->getFrameHeight() / 2,
            curseBackEft[i].frame.x,
            curseBackEft[i].frame.y);

        if (curseBackEft[i].curTime == 12)
        {
            curseBackEft[i].curTime = 0;
            curseBackEft[i].frame.x++;
        }

        curseBackEft[i].curTime++;

        //삭제
        if (curseBackEft[i].frame.x == curseBackEft[0].maxFrame)
        {
            curseBackEft.erase(curseBackEft.begin() + i);
        }
        else i++;
    }
}

void effectManager::curseRenderFront(HDC hdc)
{
    for (int i = 0; i < curseFrontEft.size();)
    {
        //렌더
        CAMERAMANAGER->FrameRender(hdc, IMAGEMANAGER->findImage("curseEffect"),
            curseFrontEft[i].pos.x - IMAGEMANAGER->findImage("curseEffect")->getFrameWidth() / 2,
            curseFrontEft[i].pos.y - IMAGEMANAGER->findImage("curseEffect")->getFrameHeight() / 2, 
            curseFrontEft[i].frame.x,
            curseFrontEft[i].frame.y);

        if (curseFrontEft[i].curTime == 12)
        {
            curseFrontEft[i].curTime = 0;
            curseFrontEft[i].frame.x++;
        }
        
        curseFrontEft[i].curTime++;

        //삭제
        if (curseFrontEft[i].frame.x == curseFrontEft[0].maxFrame)
        {
            curseFrontEft.erase(curseFrontEft.begin() + i);
        }
        else i++;
    }
}

void effectManager::setEmotionEffect(string keyName, POINT pt)
{
    tagBuyEmotion emotion;

    emotion.keyName = keyName;
    emotion.pos = pt;
    emotion.frameX = 0;
    emotion.maxFrame = IMAGEMANAGER->findImage(keyName)->getMaxFrameX();

    emotion.opacity = 255;
    emotion.curTime = 0;

    vEmotion.push_back(emotion);
}

void effectManager::emotionRender(HDC hdc)
{
    if (vEmotion.empty()) return;

    for (int i = 0; i < vEmotion.size();)
    {
        //렌더
        image* img = IMAGEMANAGER->findImage(vEmotion[i].keyName);
        CAMERAMANAGER->AlphaFrameRender(hdc, img,
            vEmotion[i].pos.x - img->getFrameWidth() / 2,
            vEmotion[i].pos.y - img->getFrameHeight() / 2,
            vEmotion[i].frameX, 0,
            vEmotion[i].opacity);

        //증가
        if (vEmotion[i].frameX != vEmotion[i].maxFrame)
        {
            vEmotion[i].curTime++;

            if (vEmotion[i].curTime == 15)
            {
                vEmotion[i].curTime = 0;

                vEmotion[i].frameX++;
            }
        }

        else // frameX == maxFrame
        {
            if (vEmotion[i].curTime != 15)
            {
                vEmotion[i].curTime++;
            }

            else
            {
                vEmotion[i].opacity -= 15;
            }
        }

        //삭제
        if (vEmotion[i].opacity == 0) vEmotion.erase(vEmotion.begin());
        else i++;
    }
}

void effectManager::setAlwaysPoint(string keyName, POINT pt)
{
    tagAlwaysPoint point;

    image* img = IMAGEMANAGER->findImage(keyName);

    //point.keyName = keyName;
    //point.frame = { 0, RANDOM->range(0,img->getMaxFrameY()) };
    //point.maxFrame = img->getMaxFrameX();
    point.keyName = keyName;
    point.curTime = 0;
    point.createTime = 10;
    point.pos = pt;

    vAlwaysPoint.push_back(point);
}

void effectManager::alwaysActive()
{
    for (int i = 0; i < vAlwaysPoint.size();)
    {
        //생성
        if (vAlwaysPoint[i].curTime == vAlwaysPoint[i].createTime)
        {
            vAlwaysPoint[i].curTime = 0;

            POINT pt = vAlwaysPoint[i].pos;
            pt.x += RANDOM->range(-19, 19);
            pt.y += RANDOM->range(-40, 25);

            createAlwaysEft(vAlwaysPoint[i].keyName, pt, 20);
        }

        vAlwaysPoint[i].curTime++;

        //이 포인터는 '삭제'가 없음
        i++;
    }
}

void effectManager::createAlwaysEft(string keyName, POINT pt, int delay)
{
    image* img = IMAGEMANAGER->findImage(keyName);

    tagAlwaysParticle particle;

    particle.curTime = 0;
    particle.delay = delay;
    particle.frame = { 0,RANDOM->range(0,img->getMaxFrameY()) };
    particle.keyName = keyName;
    particle.maxFrame = img->getMaxFrameX();
    particle.pos = pt;

    vAlwaysEft.push_back(particle);
}

void effectManager::alwaysEftRender(HDC hdc)
{
    for (int i = 0; i < vAlwaysEft.size();)
    {
        image* img = IMAGEMANAGER->findImage(vAlwaysEft[i].keyName);

        //렌더
        CAMERAMANAGER->FrameRender(hdc, img,
            vAlwaysEft[i].pos.x - img->getFrameWidth() / 2,
            vAlwaysEft[i].pos.y - img->getFrameHeight() / 2,
            vAlwaysEft[i].frame.x,
            vAlwaysEft[i].frame.y);

        //변환
        if (vAlwaysEft[i].curTime % vAlwaysEft[i].delay == 0)vAlwaysEft[i].frame.x++;
        
        vAlwaysEft[i].curTime++;

        //삭제
        if (vAlwaysEft[i].frame.x == vAlwaysEft[i].maxFrame) vAlwaysEft.erase(vAlwaysEft.begin());
        else i++;
    }
}

void effectManager::AfterimageEft(string keyName, POINT pt, POINT frame, int delay)
{
    aTime++;

    if (aTime % delay == 0)
    {
        aTime = 0;

        tagAfterimage eft;

        eft.keyName = keyName;
        eft.frame = frame;
        eft.pos = pt;
        eft.opacity = 150;

        vAfterEft.push_back(eft);
    }
}

void effectManager::AfterimageRender(HDC hdc)
{
    for (int i = 0; i < vAfterEft.size();)
    {
        //렌더
        image* img = IMAGEMANAGER->findImage(vAfterEft[i].keyName);
        CAMERAMANAGER->AlphaFrameRender(hdc, img,
            vAfterEft[i].pos.x - img->getFrameWidth()/2,
            vAfterEft[i].pos.y - img->getFrameHeight()/2,
            vAfterEft[i].frame.x,
            vAfterEft[i].frame.y,
            vAfterEft[i].opacity);

        //변환
        vAfterEft[i].opacity -= 5;

        //삭제
        if (vAfterEft[i].opacity == 0) vAfterEft.erase(vAfterEft.begin() + i);
        else i++;
    }
}

void effectManager::ultEftPlay(POINT pt, int delay)
{
    //setEffect("ultLight", pt, true, delay, false, 0, 0, 0);
    setEffect("ultLight", pt, false, false, 0, delay);
}