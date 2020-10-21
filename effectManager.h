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
	string keyName;		// 출력할 이미지

	bool isFrame;		// 프레임 이미지 인지?

	int imgCount;		// 프레임렌더에 필요한 카운트
	int frameDelay;		// 프레임 딜레이를 몇 줄지
	bool flipImg;		// 오른쪽에서 왼쪽으로 출력?

	bool isEraseSize;	// 지정 사이즈가 되면 지워질지
	float increaseSize;	// update 한번당 사이즈 증갓값
	float currentSize;	// 시작 사이즈
	float endSize;		// 최종 제한 사이즈

	bool isEraseTime;	// 시간이 지나면 지워질지, 프레임이 끝나면 지워질지
	int eraseTime;		// 몇초 뒤에 지워질지
	int currentTime;	// 현재 시간 (이건 안건드려도됨)
	int opacity;

	int maxFrame;		// 이미지의 최대 프레임 (이것도 안건드려도됨)
	POINT pos;			// 이미지를 재생시킬 위치
};

struct tagDamageEffect
{
	int frameX;			// 프레임 X 좌표
	int frameY;			// 프레임 Y 좌표

	int maxFrame;		// 이미지의 최대 프레임 (이것도 안건드려도됨)
	POINT pos;			// 이미지를 재생시킬 위치
};

struct tagPortalEffect
{
	int frameX;
	int maxFrame;

	int opacity[8];
	POINT pos;

	bool isActive;
};

struct tagCursePoint
{
	int curTime;

	POINT pt[3];
	int renderIndex;
	int createDelay;

	bool isActive;
};

struct tagCurseEffect
{
	int curTime;

	POINT frame;
	int maxFrame;

	POINT pos;
};

struct tagBuyEmotion
{
	POINT pos;

	string keyName;

	int frameX;
	int maxFrame;
	int opacity;

	int curTime;
};

struct tagAlwaysPoint
{
	string keyName;

	POINT pos;
	int curTime;
	int createTime;
};

struct tagAlwaysParticle
{
	string keyName;
	POINT pos;
	int curTime;
	int delay;
	POINT frame;
	int maxFrame;
};

struct tagAfterimage
{
	string keyName;
	POINT pos;
	POINT frame;

	int opacity;
};

class effectManager : public singletonBase <effectManager>
{
private:
	//대쉬 이펙트 관리
	vector<tagDashEffect> vDashEft;
	//maxFrame이 되면 erase되는 이펙트 관리
	vector<tagEffect> vEft;
	//데미지 이펙트 관리
	vector<tagDamageEffect> dEft;
	//포탈 이펙트 
	tagPortalEffect pEft;

	//저주 이펙트
	tagCursePoint cursePoint;
	vector<tagCurseEffect> curseBackEft;
	vector<tagCurseEffect> curseFrontEft;

	//구매 이펙트
	vector<tagBuyEmotion> vEmotion;

	//게임이 끝날 때 까지 지속되는 이펙트 포인터
	vector<tagAlwaysPoint> vAlwaysPoint;
	vector<tagAlwaysParticle> vAlwaysEft;

	vector<tagAfterimage> vAfterEft;

	vector<tagEffect> vBackEft;

	int time;			// 프레임렌더를 하기 위해 필요, 'time++' 을 해주기 위함
	int aTime;
public:
	HRESULT init();
	void pRender(HDC hdc);

	void render(HDC hdc);

	void portalRender(HDC hdc);

	void dRender(HDC hdc);

	void addImage();
	void dashEffect(MOVE direction, POINT pos);

	void setDash(string keyName, int frameY, bool flip, POINT pt);
	void setEffect(string keyName, POINT pt, bool flip = false, bool isEraseTime = false, int eraseTime = 0, int frameDelay = 5);

	void setEffect(string keyName, POINT pt, int minOpacity, int maxOpacity);

	void setPortalEffect(POINT pt);

	void setEffect(string keyName, POINT pt, bool isFrameImg , int frameDelay ,bool flip, float increaseSize, float startSize, float endSize);

	void setBackEffect(string keyName, POINT pt, int frameDelay);

	void backEftRender(HDC hdc);

	void damageEffect(POINT pt);
	void lightEffect(POINT pt, int maxEffect);

	void SetCursePoint(POINT _pt0, POINT _pt1, POINT _pt2);
	void cursePointActive();
	void curseRenderBack(HDC hdc);
	void curseRenderFront(HDC hdc);

	void setEmotionEffect(string keyName, POINT pt);
	void emotionRender(HDC hdc);

	void setAlwaysPoint(string keyName, POINT pt);
	void alwaysActive();
	void createAlwaysEft(string keyName, POINT pt, int delay);
	void alwaysEftRender(HDC hdc);
	void AfterimageEft(string keyName, POINT pt, POINT frame, int delay);
	void AfterimageRender(HDC hdc);
	void ultEftPlay(POINT pt, int delay);
	void iceBreakPlay(POINT pt, int delay);
};