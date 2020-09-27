#pragma once
#include "singletonBase.h"
#include "image.h"

class cameraManager : public singletonBase<cameraManager>
{
private:

	// CAMERAFOLLOW //
	RECT	_cameraRect;		// ���� ī�޶��� ��ǥ (RelativeX, Y�� ���Ҷ� ���)
	float	_pivotX;			// ���� ī�޶� �߽����� ����ִ� PIVOT X
	float	_pivotY;			// ���� ī�޶� �߽����� ����ִ� PIVOT Y
	float	_maxX;				// ī�޶� �ִ��� ���󰡴� �Ÿ� X
	float	_maxY;				// ī�޶� �ִ��� ���󰡴� �Ÿ� Y
	float	_minX;				// ī�޶� �ּ��� ���󰡴� �Ÿ� X
	float	_minY;				// ī�޶� �ּ��� ���󰡴� �Ÿ� X
	float	_distanceX;
	float	_distanceY;
	float	_initDistanceX;		// ���� WINSIZEX/2�� ��� (pivot ������ ��ġ)
	float	_initDistanceY;		// ���� WINSIZEY/2�� ��� (pivot ������ ��ġ)

	// CAMERASHAKE //
	float	_shakeX;			// ����ũ X
	float	_shakeY;			// ����ũ Y
	float	_tempShakeX;		// ����ũ ������ X
	float	_tempShakeY;		// ����ũ ������ Y
	float	_shakePowerX;		// ��� ���� X
	float	_shakePowerY;		// ��� ���� Y
	int		_shakeTimer;		// ��� �ð�
	int		_shakeCycleTimer;	// ��� ����Ŭ Ÿ�̸�
	int		_shakeCycle;		// ��� ����Ŭ

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
	}
	~cameraManager() {}

	HRESULT init(float pivotX, float pivotY, float maxX, float maxY, float minX, float minY, float disX, float disY);
	void update();

	void Rectangle(HDC hdc, RECT rc);
	void RectangleMake(HDC hdc, int left, int top, int width, int height);

	void Render(HDC hdc, image* ig, int destX, int destY);
	void FrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY);
	void TextDraw(HDC hdc, int destX, int destY, LPCSTR lpstring, int c);
	void StretchRender(HDC hdc, image* ig, int destX, int destY, float scale);
	void AlphaRender(HDC hdc, image* ig, int destX, int destY, BYTE alpha);
	void MovePivot(float x, float y);
	void MovePivotLerp(float x, float y, float lerpSpeed = 10.f);
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
};