#include "stdafx.h"
#include "effectManager.h"

HRESULT effectManager::init()
{
    addImage();
    time = 0;

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
    vector<tagDashEffect>::iterator iter;
    for (iter = vDashEft.begin(); iter != vDashEft.end();)
    {
        POINT position = { CAMERAMANAGER->GetRelativeX(iter->pos.x),CAMERAMANAGER->GetRelativeY(iter->pos.y) };

        IMAGEMANAGER->findImage(iter->keyName)->frameRender(hdc, position.x, position.y, iter->imgCount, iter->frameY);

        if (!iter->flipImg)
        {
            if (time % 5 == 0)iter->imgCount++;

            if (iter->imgCount == iter->maxFrame)
            {
                iter = vDashEft.erase(iter);
            }
            else iter++;
        }
        else
        {
            if (time % 5 == 0)iter->imgCount--;

            if (iter->imgCount == 0)
            {
                iter = vDashEft.erase(iter);
            }
            else iter++;
        }
    }
}

void effectManager::render(HDC hdc)
{
    vector<tagEffect>::iterator iter;
    for (iter = vEft.begin(); iter != vEft.end();)
    {
        image* img = IMAGEMANAGER->findImage(iter->keyName);

        if (iter->isFrame) // 프레임 이미지인지?
        {

            //CAMERAMANAGER->FrameRender(hdc, img,
            //    iter->pos.x - img->getFrameWidth() / 2,
            //    iter->pos.y - img->getFrameHeight() / 2,
            //    iter->imgCount, 0);

            CAMERAMANAGER->StretchFrameRender(hdc, img, 
                iter->pos.x - img->getFrameWidth() * iter->currentSize / 2,
                iter->pos.y - img->getFrameHeight() * iter->currentSize / 2,
                iter->imgCount, 0, iter->currentSize);

            if (iter->flipImg == false) // 애니메이션 정방향 출력
            {
                if (time % iter->frameDelay == 0 && iter->imgCount < iter->maxFrame) iter->imgCount++;

                if (iter->isEraseSize) // 삭제조건 : 사이즈
                {
                    iter->currentSize += iter->increaseSize;

                    float cul = abs(iter->currentSize - iter->endSize);

                    if (cul < 0.01)iter = vEft.erase(iter);
                    else iter++;
                }

                else if (iter->isEraseTime == false) // 삭제조건 : 프레임
                {
                    if (iter->imgCount == iter->maxFrame) iter = vEft.erase(iter);
                    else iter++;
                }

                else // 삭제조건 : 시간
                {
                    if (iter->currentTime == iter->eraseTime) iter = vEft.erase(iter);
                    else
                    {
                        iter->currentTime++;

                        iter++;
                    }
                }
            }

            else // 애니메이션 역방향 출력
            {
                if (time % iter->frameDelay == 0 && iter->imgCount > 0) iter->imgCount--;

                if (iter->isEraseSize) // 삭제조건 : 사이즈
                {
                    float cul = abs(iter->currentSize - iter->endSize);

                    if (cul < 0.01)iter = vEft.erase(iter);
                    else iter++;
                }

                else if (iter->isEraseTime == false) // 삭제조건 : 프레임
                {
                    if (iter->imgCount == 0) iter = vEft.erase(iter);
                    else iter++;
                }

                else // 삭제조건 : 시간
                {
                    if (iter->currentTime == iter->eraseTime) iter = vEft.erase(iter);
                    else
                    {
                        iter->currentTime++;

                        iter++;
                    }
                }
            }

        }

        else // 프레임 이미지가 아님 (size로 삭제)
        {
            if (iter->isEraseSize)
            {
                //출력
                CAMERAMANAGER->StretchRender(hdc, img,
                    iter->pos.x - (img->getWidth() * iter->currentSize / 2),
                    iter->pos.y - (img->getHeight() * iter->currentSize / 2),
                    iter->currentSize);

                //사이즈 변경
                iter->currentSize += iter->increaseSize;

                //삭제
                float cul = abs(iter->currentSize - iter->endSize);

                if (cul < 0.01) iter = vEft.erase(iter);
                else iter++;
            }
            
            else // time del
            {
                //출력
                CAMERAMANAGER->AlphaRender(hdc, img, 
                    iter->pos.x - (img->getWidth() / 2),
                    iter->pos.y - (img->getHeight() / 2),
                    iter->opacity);
                
                //이동
                if (time % 2 == 0) iter->pos.y--;

                //투명도 변경
                if(iter->opacity > 0)iter->opacity--;

                //삭제
                if (iter->opacity == 0) iter = vEft.erase(iter);

                else iter++;
            }
        }
    }
}

void effectManager::portalRender(HDC hdc)
{
    if (pEft.isActive)
    {
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
}

void effectManager::dRender(HDC hdc)
{
    vector<tagDamageEffect>::iterator iter = dEft.begin();
    for (iter; iter != dEft.end();)
    {
        image* img = IMAGEMANAGER->findImage("damageEffect");
        CAMERAMANAGER->FrameRender(hdc, img, 
            iter->pos.x - img->getFrameWidth()/2,
            iter->pos.y - img->getFrameHeight()/2, 
            iter->frameX, iter->frameY);

        if (time % 5 == 0) iter->frameX++;

        if (iter->frameX == iter->maxFrame)
        {
            iter = dEft.erase(iter);
        }
        else iter++;
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
    //damage
    IMAGEMANAGER->addFrameImage("damageEffect", "Images/effect/monster/damageEffect.bmp", 300, 300, 4, 4);
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