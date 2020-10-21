#include "stdafx.h"
#include "bossSkill.h"

HRESULT bossSkill::init(float x, float y, int deleteTime, int type)
{
	//_player = new player;

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

		temp = RectMake(_posX + 29, _posY + 94, 60, 60);
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
	case 2:
		IMAGEMANAGER->addFrameImage("bossCircle", "resource/boss/bosscircle.bmp", 3584, 256, 14, 1, true, RGB(255, 0, 255));

		_bossCircleImg = IMAGEMANAGER->findImage("bossCircle");

		_frameX = 0;
		_frameY = 0;
		break;
	case 3:
		temp = RectMake(_posX + 28, _posY + 28, 200, 200);
		break;
	case 4:
		IMAGEMANAGER->addFrameImage("bossblaze", "resource/player/blaze.bmp", 288, 96, 3, 1, true, RGB(255, 0, 255));

		_bossBlazeImg = IMAGEMANAGER->findImage("bossblaze");
		temp = RectMake(_posX, _posY, 96, 96);

		_frameX = 0;
		_frameY = 0;
		break;
	case 5:
		IMAGEMANAGER->addFrameImage("bossthunder", "resource/boss/thunder.bmp", 2100, 700, 3, 1, true, RGB(255, 0, 255));
		_thunderImg = IMAGEMANAGER->findImage("bossthunder");

		temp = RectMake(_posX, _posY, 700, 700);

		_frameX = 0;
		_frameY = 0;
		break;
	case 6:
		IMAGEMANAGER->addFrameImage("bosswind", "resource/boss/chaosCircle.bmp", 1788 * 3, 116 * 3, 12, 1, true, RGB(255, 0, 255));
		_windImg = IMAGEMANAGER->findImage("bosswind");

		temp = RectMake(_posX, _posY, 447, 348);

		_frameX = 0;
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
	this->collCheck();
}

void bossSkill::render(HDC hdc)
{
	switch (_type)
	{
	case 0:
		//_wallImg->frameRender(hdc, _posX, _posY, _frameX, _frameY);
		CAMERAMANAGER->FrameRender(hdc, _wallImg, _posX, _posY, _frameX, _frameY);
		//CAMERAMANAGER->Rectangle(hdc, temp);
		break;
	case 1:
		//_niddleImg->frameRender(hdc, _posX, _posY, _frameX, _frameY);
		CAMERAMANAGER->FrameRender(hdc, _niddleImg, _posX, _posY, _frameX, _frameY);
		//CAMERAMANAGER->Rectangle(hdc, temp);
		break;
	case 2:
		CAMERAMANAGER->AlphaFrameRender(hdc, _bossCircleImg, _posX, _posY, _frameX, _frameY, 7);
		//CAMERAMANAGER->Rectangle(hdc, temp);
		break;
	case 3:
		CAMERAMANAGER->Rectangle(hdc, temp);
		break;
	case 4:
		CAMERAMANAGER->FrameRender(hdc, _bossBlazeImg, _posX, _posY, _frameX, _frameY);
		CAMERAMANAGER->Rectangle(hdc, temp);
		break;
	case 5:
		CAMERAMANAGER->FrameRender(hdc, _thunderImg, _posX, _posY, _frameX, _frameY);
		CAMERAMANAGER->Rectangle(hdc, temp);
		break;
	case 6:
		CAMERAMANAGER->FrameRender(hdc, _windImg, _posX, _posY, _frameX, _frameY);
		CAMERAMANAGER->Rectangle(hdc, temp);
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
	case 2:
		_timer++;
		if (_timer % 2 == 0) {
			_frameX++;
			if (_frameX > 14) {
				_frameX = 14;
			}
		}
		break;
	case 4:
		_timer++;
		if (_timer % 2 == 0) {
			_frameX++;
			if (_frameX > 3) {
				_frameX = 0;
			}
		}
		break;
	case 5:
		_timer++;
		if (_timer % 2 == 0) {
			_frameX++;
			if (_frameX > 3) {
				_frameX = 0;
			}
		}
	case 6:
		_timer++;
		if (_timer % 2 == 0) {
			_frameX++;
			if (_frameX > 12) {
				_frameX = 12;
			}
		}
		break;
	}

}

void bossSkill::collCheck()
{
	RECT _temp;
	/*if (IntersectRect(&_temp, &temp, &_player->getRect)) {
		cout << "Ãæµ¹" << endl;
	}*/
}
