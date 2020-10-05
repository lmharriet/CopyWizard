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
	int imgCount;		// 프레임렌더에 필요한 카운트
	bool flipImg;		// 오른쪽에서 왼쪽으로 출력?

	bool isEraseTime;	// 시간이 지나면 지워질지, 프레임이 끝나면 지워질지
	int eraseTime;		// 몇초 뒤에 지워질지
	int currentTime;	// 현재 시간 (이건 안건드려도됨)

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
class effectManager : public singletonBase <effectManager>
{
private:
	vector<tagDashEffect> vDashEft;
	
	//maxFrame이 되면 erase되는 이펙트 관리
	vector<tagEffect> vEft;
	//데미지 이펙트 관리
	vector<tagDamageEffect> dEft;

	int time;			// 프레임렌더를 하기 위해 필요, 'time++' 을 해주기 위함
public:
	HRESULT init();
	void pRender(HDC hdc);

	void render(HDC hdc);

	void dRender(HDC hdc);

	void addImage();
	void dashEffect(MOVE direction, POINT pos);

	void setDash(string keyName, int frameY, bool flip, POINT pt);
	void setEffect(string keyName, POINT pt, bool flip = false, bool isEraseTime = false, int eraseTime = 0);

	void damageEffect(POINT pt);
};