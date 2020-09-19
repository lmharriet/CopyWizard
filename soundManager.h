#pragma once
#include "singletonBase.h"
/*FMOD ����ϱ� ���� ���̺귯�� �߰�*/
#include "FMOD/inc/fmod.hpp"
#pragma comment (lib, "FMOD/lib/fmodex_vc.lib")
using namespace FMOD;
//=============================================================
//	## soundManager ## (���� �Ŵ���)
//=============================================================

//����� ������� (���尹���� 30�� �Ѿ�� ũ�⸦ �� �÷���� �Ѵ�)
#define SOUNDBUFFER 30

class soundManager : public singletonBase <soundManager>
{
private:
	typedef map<string, Sound**> arrSound;
	typedef map<string, Sound**>::iterator arrSoundIter;

private:
	System* _system;		//�ý��� Ŭ����
	Sound** _sound;			//���� Ŭ����
	Channel** _channel;		//ä�� Ŭ����

	arrSound _mTotalSound;	//�ʿ� ��Ƶ� �����

public:
	HRESULT init();
	void release();
	void update();

	//���� �߰� (Ű��, �����̸�, BGM?, ������ų�ų�)
	void addSound(string keyName, string soundName, bool bgm = false, bool loop = false);

	//���� ���
	void play(string keyName, float volume = 1.0f); //0.0f(min) ~ 1.0f(max)
	//���� ����
	void stop(string keyName);
	//���� �Ͻ�����
	void pause(string keyName);
	//���� �ٽ����
	void resume(string keyName);

	//�÷��� ���̳�?
	bool isPlaySound(string keyName);
	//�Ͻ����� ���̳�?
	bool isPauseSound(string keyName);

};