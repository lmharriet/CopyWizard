#pragma once
#include "singletonBase.h"
#include "image.h"

class cameraManager : public singletonBase<cameraManager>
{
private:

	// CAMERAFOLLOW //
	RECT	_cameraRect;		// 현재 카메라의 좌표 (RelativeX, Y를 구할때 사용)
	float	_pivotX;			// 현재 카메라가 중심으로 잡고있는 PIVOT X
	float	_pivotY;			// 현재 카메라가 중심으로 잡고있는 PIVOT Y
	float	_maxX;				// 카메라가 최대한 따라가는 거리 X
	float	_maxY;				// 카메라가 최대한 따라가는 거리 Y
	float	_minX;				// 카메라가 최소한 따라가는 거리 X
	float	_minY;				// 카메라가 최소한 따라가는 거리 X
	float	_distanceX;
	float	_distanceY;
	float	_initDistanceX;		// 보통 WINSIZEX/2를 사용 (pivot 고정할 위치)
	float	_initDistanceY;		// 보통 WINSIZEY/2를 사용 (pivot 고정할 위치)

	// CAMERASHAKE //
	float	_shakeX;			// 쉐이크 X
	float	_shakeY;			// 쉐이크 Y
	float	_tempShakeX;		// 쉐이크 러프용 X
	float	_tempShakeY;		// 쉐이크 러프용 Y
	float	_shakePowerX;		// 흔들 세기 X
	float	_shakePowerY;		// 흔들 세기 Y
	int		_shakeTimer;		// 흔들 시간
	int		_shakeCycleTimer;	// 흔들 사이클 타이머
	int		_shakeCycle;		// 흔들 사이클

	// CUTSCENE //
	int _cutSceneTimer;	// 컷씬 타이머
	int _returnTime;	// 원래 피봇으로 돌아오는 시간
	float _startX;		// 시작점 x좌표
	float _startY;		// 시작점 y좌표
	float _aimX;		// 목적지 x좌표
	float _aimY;		// 목적지 y좌표
	bool _isLerp_StoA;	// 시작점 -> 목적지 까지의 lerp 효과 유무
	bool _isLerp_AtoS;	// 목적지 -> 시작점 까지의 lerp 효과 유무
	float _lerpSpeed;	// lerp 효과 속도
	bool _isCutScene;	// 컷씬 재생중?

	void Shaker();

public:

	cameraManager() {
		_cameraRect = { 0,0,0,0 };
		_pivotX = 0.f;
		_pivotY = 0.f;
		_maxX = 0.f;
		_maxY = 0.f;
		_minX = 0.f;
		_minY = 0.f;
		_distanceX = 0.f;
		_distanceY = 0.f;
		_initDistanceX = 0.f;
		_initDistanceY = 0.f;

		_cutSceneTimer = 0;
		_returnTime = 0;
		_startX = 0;
		_startY = 0;
		_aimX = 0;
		_aimY = 0;
		_isLerp_StoA = 0;
		_isLerp_AtoS = 0;
		_lerpSpeed = 0;
		_isCutScene = 0;
	}
	~cameraManager() {}

	HRESULT init(float pivotX, float pivotY, float maxX, float maxY, float minX, float minY, float disX, float disY);
	void update();

	void Rectangle(HDC hdc, RECT rc);
	void Ellipse(HDC hdc, RECT rc);
	void RectangleMake(HDC hdc, int left, int top, int width, int height);
	void RectangleMakeCenter(HDC hdc, int x, int y, int width, int height);

	void Render(HDC hdc, image* ig, int destX, int destY);
	void FrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY);
	void AlphaFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, BYTE alpha);

	void StretchFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, float scale);

	void StretchRender(HDC hdc, image* ig, int destX, int destY, float scale);
	void RotateRender(HDC hdc, image* ig, int destX, int destY, float angle);

	void RotateFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, float angle);

	void TextDraw(HDC hdc, int destX, int destY, LPCSTR lpstring, int c);
	void AlphaRender(HDC hdc, image* ig, int destX, int destY, BYTE alpha);
	void MovePivot(float x, float y);
	void MovePivotLerp(float x, float y, float lerpSpeed = 10.f);
	void setCutScene(float startX, float startY, float aimX, float aimY, bool isLerp_START_TO_AIM, bool isLerp_AIM_TO_START, int cutTime, int returnTime, float lerpSpeed);
	void playCutScene();
	void Shake(float shakePowerX, float shakePowerY, int shakeTime, int shakeCycle = 0);
	void ZoomInOut(HDC hdc, int destX, int destY, int sourX, int sourY, float scale);
	int GetRelativeX(float x) { return x - _cameraRect.left; }
	int GetRelativeY(float y) { return y - _cameraRect.top; }
	int GetAbsoluteX(float x) { return x + _cameraRect.left; }
	int GetAbsoluteY(float y) { return y + _cameraRect.top; }
	float getPivotX() { return _pivotX; }
	float getPivotY() { return _pivotY; }
	float getDistanceX() { return _distanceX; }
	float getDistanceY() { return _distanceY; }
	RECT getRect() { return _cameraRect; }

	float getMinX() { return _minX; }
	float getMinY() { return _minY; }

	bool getIsCutScene() { return _isCutScene; }
};