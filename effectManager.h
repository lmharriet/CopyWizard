#pragma once
#include "singletonBase.h"
#include "unitNode.h"

struct tagDashEffect
{
	string keyName;		// 출력할 이미지
	int imgCount;		// 프레임렌더에 필요한 카운트
	bool flipImg;		// 이미지가 반대로있냐? (오른쪽에서 왼쪽으로 그려진 이미지를 렌더할 때)
	int frameY;			// 프레임 Y의 위치
	int maxFrame;		// 최대 X 프레임 (이미지가 최대 프레임이 되면 벡터에서 ERASE)
	POINT pos;			// 출력할 위치
};

struct tagEffect
{
	string keyName;
	int imgCount;
	bool flipImg;

	bool isEraseTime;
	int eraseTime;
	int currentTime;

	int maxFrame;
	POINT pos;
};
class effectManager : public singletonBase <effectManager>
{
private:
	vector<tagDashEffect> vDashEft;
	
	//maxFrame이 되면 erase되는 이펙트 관리
	vector<tagEffect> vEft;

	int time;			// 프레임렌더를 하기 위해 필요, 'time++' 을 해주기 위함
public:
	HRESULT init();
	void pRender(HDC hdc);

	void render(HDC hdc);

	void addImage();
	void dashEffect(MOVE direction, POINT pos);

	void setDash(string keyName, int frameY, bool flip, POINT pt);
	void setEffect(string keyName, POINT pt, bool flip = false, bool isEraseTime = false, int eraseTime = 0);
};