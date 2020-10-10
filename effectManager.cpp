#include "stdafx.h"
#include "effectManager.h"

HRESULT effectManager::init()
{
    addImage();
    time = 0;
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

        if (iter->isFrame) // ������ �̹�������?
        {

            //CAMERAMANAGER->FrameRender(hdc, img,
            //    iter->pos.x - img->getFrameWidth() / 2,
            //    iter->pos.y - img->getFrameHeight() / 2,
            //    iter->imgCount, 0);

            CAMERAMANAGER->StretchFrameRender(hdc, img, 
                iter->pos.x - img->getFrameWidth() * iter->currentSize / 2,
                iter->pos.y - img->getFrameHeight() * iter->currentSize / 2,
                iter->imgCount, 0, iter->currentSize);

            if (iter->flipImg == false) // �ִϸ��̼� ������ ���
            {
                if (time % iter->frameDelay == 0 && iter->imgCount < iter->maxFrame) iter->imgCount++;

                if (iter->isEraseSize) // �������� : ������
                {
                    iter->currentSize += iter->increaseSize;

                    float cul = abs(iter->currentSize - iter->endSize);

                    if (cul < 0.01)iter = vEft.erase(iter);
                    else iter++;
                }

                else if (iter->isEraseTime == false) // �������� : ������
                {
                    if (iter->imgCount == iter->maxFrame) iter = vEft.erase(iter);
                    else iter++;
                }

                else // �������� : �ð�
                {
                    if (iter->currentTime == iter->eraseTime) iter = vEft.erase(iter);
                    else
                    {
                        iter->currentTime++;

                        iter++;
                    }
                }
            }

            else // �ִϸ��̼� ������ ���
            {
                if (time % iter->frameDelay == 0 && iter->imgCount > 0) iter->imgCount--;

                if (iter->isEraseSize) // �������� : ������
                {
                    float cul = abs(iter->currentSize - iter->endSize);

                    if (cul < 0.01)iter = vEft.erase(iter);
                    else iter++;
                }

                else if (iter->isEraseTime == false) // �������� : ������
                {
                    if (iter->imgCount == 0) iter = vEft.erase(iter);
                    else iter++;
                }

                else // �������� : �ð�
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

        else // ������ �̹����� �ƴ� (size�� ����)
        {
            //���
            CAMERAMANAGER->StretchRender(hdc, img, 
                iter->pos.x - (img->getWidth() * iter->currentSize / 2),
                iter->pos.y - (img->getHeight() * iter->currentSize / 2),
                iter->currentSize);

            //������ ����
            iter->currentSize += iter->increaseSize;

            //����
            float cul = abs(iter->currentSize - iter->endSize);

            if (cul < 0.01) iter = vEft.erase(iter);
            else iter++;
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

//����Ʈ�� ���� ��Ű�� �� addImage(frame)�� �Ǿ��ִ��� Ȯ���ؾ��Ѵ�.
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

    effect.currentSize = 1.f;

    if (!flip) effect.imgCount = 0;
    else effect.imgCount = effect.maxFrame;

    vEft.push_back(effect);
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

    cout << effect.imgCount << '\n';
    cout << effect.maxFrame << '\n';

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