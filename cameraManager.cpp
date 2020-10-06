#include "stdafx.h"
#include "cameraManager.h"


HRESULT cameraManager::init(float pivotX, float pivotY, float maxX, float maxY, float minX, float minY, float disX, float disY)
{
	// MOVE //
	_pivotX = pivotX;
	_pivotY = pivotY;
	_maxX = maxX;
	_maxY = maxY;
	_minX = minX;
	_minY = minY;
	_initDistanceX = _distanceX = disX;
	_initDistanceY = _distanceY = disY;

	// SHAKE // 
	_shakeTimer = 0;
	_shakePowerX = 0;
	_shakePowerY = 0;
	_shakeX = 0;
	_shakeY = 0;
	_tempShakeX = 0;
	_tempShakeY = 0;

	_cameraRect = RectMake(_pivotX - disX, _pivotY - disY, WINSIZEX, WINSIZEY);

	return S_OK;
}

void cameraManager::update()
{
	Shaker();
	playCutScene();
}

void cameraManager::MovePivot(float x, float y)
{
	_pivotX = x;
	_pivotY = y;
	_cameraRect = RectMake(_pivotX - _initDistanceX, _pivotY - _initDistanceY, WINSIZEX, WINSIZEY);
	//_pivotX - _initDistanceX -> _initDistance = Pivot이 화면에서 표시될 위치, _pivotX = 현재 따라갈 X의 좌표
		//_pivotX - _initDistanceX =           _INITDISTANCE = WINSIZEX/2 , _PIVOTX = WINSIZEX/2 + 100
	if (_cameraRect.left <= _minX) // LEFT가 최소가 되면
	{
		_cameraRect.left = _minX; // 카메라 렉트를 최소에 맞춰준다
		_cameraRect.right = _minX + WINSIZEX;
	}
	if (_cameraRect.right >= _maxX) // RIGHT가 최대가 되면
	{
		_cameraRect.right = _maxX; // 카메라 렉트를 최대에 맞춰준다
		_cameraRect.left = _maxX - WINSIZEX;
	}
	if (_cameraRect.top <= _minY)
	{
		_cameraRect.top = _minY;
		_cameraRect.bottom = _minY - WINSIZEY;
	}
	if (_cameraRect.bottom >= _maxY)
	{
		_cameraRect.bottom = _maxY;
		_cameraRect.top = _maxY - WINSIZEY;
	}

	// SHAKE 관련
	OffsetRect(&_cameraRect, _shakeX, _shakeY);
}

// MOVEPIVOTLERP :: 부드럽게 움직이게 한다
void cameraManager::MovePivotLerp(float x, float y, float lerpSpeed)
{

	MovePivot(
		(x - _pivotX) / lerpSpeed + _pivotX,
		(y - _pivotY) / lerpSpeed + _pivotY
	);
}

void cameraManager::setCutScene(float startX, float startY, float aimX, float aimY, 
	bool isLerp_START_TO_AIM, bool isLerp_AIM_TO_START,int cutTime ,int returnTime, float lerpSpeed)
{
	_startX = startX;
	_startY = startY;
	_aimX = aimX;
	_aimY = aimY;
	_isLerp_StoA = isLerp_START_TO_AIM;
	_isLerp_AtoS = isLerp_AIM_TO_START;
	_cutSceneTimer = cutTime;
	_returnTime = returnTime;
	_lerpSpeed = lerpSpeed;
}

void cameraManager::playCutScene()
{
	if (_cutSceneTimer > 0)
	{
		_isCutScene = true;
		_cutSceneTimer--;

		if (_cutSceneTimer > _returnTime)
		{
			if (_isLerp_StoA) MovePivotLerp(_aimX, _aimY, _lerpSpeed);
			else MovePivot(_aimX, _aimY);
		}

		else
		{
			if (_isLerp_AtoS) MovePivotLerp(_startX, _startY, _lerpSpeed);
			else  MovePivot(_startX, _startY);
		}

		if (_cutSceneTimer == 0)
		{
			_isCutScene = false;
			_returnTime = 0;
			_lerpSpeed = 0;
		}
	}
}

void cameraManager::Shake(float shakePowerX, float shakePowerY, int shakeTime, int shakeCycle)
{
	_shakePowerX = shakePowerX;
	_shakePowerY = shakePowerY;
	_shakeTimer = shakeTime;
	_shakeCycleTimer = _shakeCycle = shakeCycle;
}

void cameraManager::ZoomInOut(HDC hdc, int destX, int destY, int sourX, int sourY, float scale)
{
	SetStretchBltMode(hdc, COLORONCOLOR);

	StretchBlt(
		hdc,
		destX, destY,
		(WINSIZEX + sourX / 2) * scale, (WINSIZEY + sourY / 2) * scale,
		hdc,
		GetRelativeX(_pivotX) - sourX / 2, GetRelativeY(_pivotY) - sourY / 2,
		WINSIZEX, WINSIZEY,
		SRCCOPY
	);
}

void cameraManager::Shaker()
{
	if (_shakeTimer > 0)
	{
		// _shakeTimer는 쉐이크가 일어날 시간
		// _shakeCycleTimer는 n프레임마다 한번씩 쉐이크가 일어나게
		_shakeTimer--;
		_shakeCycleTimer--;

		if (_shakeCycleTimer < 0)
		{
			if (_shakeCycle == 0)
			{
				_shakeX = RANDOM->range(-_shakePowerX, _shakePowerX);
				_shakeY = RANDOM->range(-_shakePowerY, _shakePowerY);
			}

			else
			{
				_shakeX = _tempShakeX;
				_shakeY = _tempShakeY;

				_shakeX = (_tempShakeX - _shakeX) / _shakeCycle + _shakeX;
				_shakeY = (_tempShakeY - _shakeY) / _shakeCycle + _shakeY;

				_tempShakeX = RANDOM->range(-_shakePowerX, _shakePowerX);
				_tempShakeY = RANDOM->range(-_shakePowerY, _shakePowerY);

			}

			_shakeCycleTimer = _shakeCycle;
		}

		else // SHAKE를 주기마다 자연스럽게 이동하도록
		{
			_shakeX = (_tempShakeX - _shakeX) / _shakeCycle + _shakeX;
			_shakeY = (_tempShakeY - _shakeY) / _shakeCycle + _shakeY;
			// LERP
		}

		if (_shakeTimer == 0)
		{
			_shakePowerX = 0;
			_shakePowerY = 0;
			_shakeX = 0;
			_shakeY = 0;
		}
	}
}

void cameraManager::Render(HDC hdc, image* ig, int destX, int destY)
{
	ig->render(hdc, GetRelativeX(destX), GetRelativeY(destY));
}

void cameraManager::FrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY)
{
	ig->frameRender(hdc, GetRelativeX(destX), GetRelativeY(destY), frameX, frameY);
}

void cameraManager::AlphaFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, BYTE alpha)
{
	ig->alphaFrameRender(hdc, GetRelativeX(destX), GetRelativeY(destY), frameX, frameY, alpha);
}

void cameraManager::StretchFrameRender(HDC hdc, image* ig, int destX, int destY,int frameX, int frameY, float scale)
{
	ig->frameRender(hdc, GetRelativeX(destX), GetRelativeY(destY), frameX, frameY, scale);
	//ig->stretchFrameRender(hdc, GetRelativeX(destX), GetRelativeY(destY), frameX, frameY, scale);
}

void cameraManager::AlphaRender(HDC hdc, image* ig, int destX, int destY, BYTE alpha)
{
	ig->alphaRender(hdc, GetRelativeX(destX), GetRelativeY(destY), alpha);
}

void cameraManager::Rectangle(HDC hdc, RECT rc)
{
	::Rectangle(hdc, GetRelativeX(rc.left), GetRelativeY(rc.top), GetRelativeX(rc.right), GetRelativeY(rc.bottom));
}

void cameraManager::Ellipse(HDC hdc, RECT rc)
{
	::Ellipse(hdc, GetRelativeX(rc.left), GetRelativeY(rc.top), GetRelativeX(rc.right), GetRelativeY(rc.bottom));
}

void cameraManager::RectangleMake(HDC hdc, int left, int top, int width, int height)
{
	::RectangleMake(hdc, GetRelativeX(left), GetRelativeY(top), width, height);
}

void cameraManager::RectangleMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	::RectangleMakeCenter(hdc, GetRelativeX(x), GetRelativeY(y), width, height);
}

void cameraManager::TextDraw(HDC hdc, int destX, int destY, LPCSTR lpstring, int c)
{
	TextOut(hdc, GetRelativeX(destX), GetRelativeY(destY), lpstring, c);
}
