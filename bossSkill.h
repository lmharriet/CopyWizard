#pragma once
class bossSkill
{
private:
	RECT temp;
	float _posX;
	float _posY;
	int _count;
	int _deleteTime;
	int _type;
	bool _isHit;
	image* _wallImg;
	image* _niddleImg;
	image* _bossCircleImg;
	image* _bossBlazeImg;
	image* _thunderImg;
	image* _windImg;
	image* _iceImg;

	int _timer;
	int _frameX;
	int _frameY;
public:
	HRESULT init(float x, float y, int deleteTime, int type);
	void release();
	void update();
	void render(HDC hdc);

	void animation();
	void collCheck();

	int getCount() {
		return _count;
	}
	int getDeleteTime() {
		return _deleteTime;
	}
	int getType() {
		return _type;
	}
	RECT getRect() {
		return temp;
	}
};

