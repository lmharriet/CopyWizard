#include "stdafx.h"
#include "bossSkill.h"

HRESULT bossSkill::init(float x, float y, int deleteTime, int type)
{
	_posX = x;
	_posY = y;
	_deleteTime = deleteTime;

	_count = 0;
	_type = type;

	_timer = 0;

	switch (_type)
	{
	case 0:
		IMAGEMANAGER->addFrameImage("wall", "resource/boss/2.bmp", 705, 189, 6, 1, true, RGB(255, 0, 255));

		_wallImg = IMAGEMANAGER->findImage("wall");

		temp = RectMake(_posX + 35, _posY + 117, 48, 48);
		_frameX = 0;
		_frameY = 0;
		break;
	case 1:
		IMAGEMANAGER->addFrameImage("niddle", "resource/boss/stoneWall.bmp", 240, 69, 4, 1, true, RGB(255, 0, 255));

		_niddleImg = IMAGEMANAGER->findImage("niddle");

		temp = RectMake(_posX + 15, _posY + 39, 30, 30);
		_frameX = 3;
		_frameY = 0;
		break;
	}

	return S_OK;
}

void bossSkill::release()
{
}

void bossSkill::update()
{
	_count++;
	this->animation();
}

void bossSkill::render(HDC hdc)
{
	switch (_type)
	{
	case 0:
		//_wallImg->frameRender(hdc, _posX, _posY, _frameX, _frameY);
		CAMERAMANAGER->FrameRender(hdc, _wallImg, _posX, _posY, _frameX, _frameY);
		break;
	case 1:
		//_niddleImg->frameRender(hdc, _posX, _posY, _frameX, _frameY);
		CAMERAMANAGER->FrameRender(hdc, _niddleImg, _posX, _posY, _frameX, _frameY);
		break;
	}
}

void bossSkill::animation()
{
	switch (_type)
	{
	case 0:
		_timer++;
		if (_timer % 6 == 0) {
			_frameX++;
			if (_frameX > 6) {
				_frameX = 6;
			}
		}
		break;
	case 1:
		_timer++;
		if (_timer % 6 == 0) {
			_frameX--;
			if (_frameX < 0) {
				_frameX = 0;
			}
		}
		break;
	}

}
