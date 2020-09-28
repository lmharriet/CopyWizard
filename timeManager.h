#pragma once
#include "singletonBase.h"
//==================================================================
//		## timeManager ## (타임 매니져)
//==================================================================

class timeManager : public singletonBase <timeManager>
{
private:
	bool _isHardware;				//고성능 타이머를 지원하냐?
	float _timeScale;				//시간 정밀도 (1초에 몇번이나 카운팅이 가능항지 여부)
	float _deltaTime;				//한프레임당 경과량
	__int64 _curTime;				//현재시간
	__int64 _lastTime;				//마지막시간
	__int64 _periodFrequency;		//시간주기 (1초에 몇번이나 카운트되는지)

	unsigned long _frameRate;		//FPS
	unsigned long _FPSFrameCount;	//FPS 카운트
	float _FPSTimeElapsed;			//FPS 경과량
	float _time;						//전체 경과시간

public:
	HRESULT init(void);
	void release(void);
	void update(float lockFPS);
	void render(HDC hdc);

	//한프레임당 경과시간 가져오기 (유니티엔진 Time.deltaTime)
	float deltaTime(void) { return _deltaTime; }
	//전체 경과시간 가져오기 (유니티엔진 Time.time)
	float time(void) { return _time; }

};
