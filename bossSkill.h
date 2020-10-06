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
	image* _wallImg;
	image* _niddleImg;

	int _timer;
	int _frameX;
	int _frameY;
public:
	HRESULT init(float x, float y, int deleteTime, int type);
	void release();
	void update();
	void render(HDC hdc);

	void animation();

	int getCount() {
		return _count;
	}
	int getDeleteTime() {
		return _deleteTime;
	}
};

