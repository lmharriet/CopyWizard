#include "stdafx.h"
#include "soundManager.h"

HRESULT soundManager::init()
{
	//FMOD 시스템을 생성한다
	System_Create(&_system);

	//사운드, 채널수 설정
	_system->init(SOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드, 채널 클래스 동적할당
	_sound = new Sound*[SOUNDBUFFER];
	_channel = new Channel*[SOUNDBUFFER];

	//메모리 초기화
	memset(_sound, 0, sizeof(Sound*) * SOUNDBUFFER);
	memset(_channel, 0, sizeof(Channel*) * SOUNDBUFFER);

	return S_OK;
}

void soundManager::release()
{
	//사운드 및 채널 삭제
	if (_sound != NULL || _channel != NULL)
	{
		for (int i = 0; i < SOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//메모리 지우기
	SAFE_DELETE_ARRAY(_sound);
	SAFE_DELETE_ARRAY(_channel);

	//시스템 닫기
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void soundManager::update()
{
	//메인게임에 사운드매니져를 업데이트 꼭 해줘야 한다
	//사운드 관련 총괄담당을 한다
	//사운드 시스템은 볼륨이 변경되거나, 재생이 끝난 후 사운드를 변경하는 등
	//사운드 전반적으로 변경이 이루어 질때 즉각적으로 처리해준다
	_system->update();
}

void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	if (loop) //일반적으로 BGM
	{
		if (bgm)
		{
			//사운드 파일을 미리 업로딩해두고 사용한다
			//한개의 파일만 재생가능
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
		else //거의 사용안함
		{
			//사운드 파일을 그때그때 실시간으로 로딩한다
			//따라서 효과음의 파일 크기가 클경우 버퍼가 생길 수도 있다
			//여러개의 파일 동시재생 가능하기때문에 효과음에 사용하기적당하다
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
	}
	else //일반적으로 효과음
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSound.size()]);
	}

	//맵에 사운드를 키값과 함께 담아둔다
	_mTotalSound.insert(make_pair(keyName, &_sound[_mTotalSound.size()]));
}

void soundManager::play(string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//볼륨세팅
			_channel[count]->setVolume(volume);
		}
	}
}

void soundManager::stop(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 정지
			_channel[count]->stop();
		}
	}
}

void soundManager::pause(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 일시정지
			_channel[count]->setPaused(true);
		}
	}
}

void soundManager::resume(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 다시재생
			_channel[count]->setPaused(false);
		}
	}
}

bool soundManager::isPlaySound(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 일시정지
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

bool soundManager::isPauseSound(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 일시정지
			_channel[count]->getPaused(&isPause);
			break;
		}
	}
	return isPause;
}