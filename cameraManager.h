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

	// CUTSCENE //
	int _cutSceneTimer;	// �ƾ� Ÿ�̸�
	int _returnTime;	// ���� �Ǻ����� ���ƿ��� �ð�
	float _startX;		// ������ x��ǥ
	float _startY;		// ������ y��ǥ
	float _aimX;		// ������ x��ǥ
	float _aimY;		// ������ y��ǥ
	bool _isLerp_StoA;	// ������ -> ������ ������ lerp ȿ�� ����
	bool _isLerp_AtoS;	// ������ -> ������ ������ lerp ȿ�� ����
	float _lerpSpeed;	// lerp ȿ�� �ӵ�
	bool _isCutScene;	// �ƾ� �����?

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

	void TextDraw(HDC hdc, int destX, int destY, LPCSTR lpstring, int c);
	void StretchRender(HDC hdc, image* ig, int destX, int destY, float scale);
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