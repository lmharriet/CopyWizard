#pragma once
#include "singletonBase.h"
/*FMOD 사용하기 위해 라이브러리 추가*/
#include "FMOD/inc/fmod.hpp"
#pragma comment (lib, "FMOD/lib/fmodex_vc.lib")
using namespace FMOD;
//=============================================================
//	## soundManager ## (사운드 매니져)
//=============================================================

//사용할 사운드버퍼 (사운드갯수가 30개 넘어가면 크기를 더 늘려줘야 한다)
#define SOUNDBUFFER 30

class soundManager : public singletonBase <soundManager>
{
private:
	typedef map<string, Sound**> arrSound;
	typedef map<string, Sound**>::iterator arrSoundIter;

private:
	System* _system;		//시스템 클래스
	Sound** _sound;			//사운드 클래스
	Channel** _channel;		//채널 클래스

	arrSound _mTotalSound;	//맵에 담아둘 사운드들

public:
	HRESULT init();
	void release();
	void update();

	//사운드 추가 (키값, 파일이름, BGM?, 루프시킬거냐)
	void addSound(string keyName, string soundName, bool bgm = false, bool loop = false);

	//사운드 재생
	void play(string keyName, float volume = 1.0f); //0.0f(min) ~ 1.0f(max)
	//사운드 정지
	void stop(string keyName);
	//사운드 일시정지
	void pause(string keyName);
	//사운드 다시재생
	void resume(string keyName);

	//플레이 중이냐?
	bool isPlaySound(string keyName);
	//일시정지 중이냐?
	bool isPauseSound(string keyName);

};