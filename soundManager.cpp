#include "stdafx.h"
#include "soundManager.h"

HRESULT soundManager::init()
{
	//FMOD �ý����� �����Ѵ�
	System_Create(&_system);

	//����, ä�μ� ����
	_system->init(SOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//����, ä�� Ŭ���� �����Ҵ�
	_sound = new Sound*[SOUNDBUFFER];
	_channel = new Channel*[SOUNDBUFFER];

	//�޸� �ʱ�ȭ
	memset(_sound, 0, sizeof(Sound*) * SOUNDBUFFER);
	memset(_channel, 0, sizeof(Channel*) * SOUNDBUFFER);

	_volumeBGM = 0.5f;
	_volumeSFX = 0.5f;

	return S_OK;
}

void soundManager::release()
{
	//���� �� ä�� ����
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

	//�޸� �����
	SAFE_DELETE_ARRAY(_sound);
	SAFE_DELETE_ARRAY(_channel);

	//�ý��� �ݱ�
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void soundManager::update()
{
	//���ΰ��ӿ� ����Ŵ����� ������Ʈ �� ����� �Ѵ�
	//���� ���� �Ѱ������ �Ѵ�
	//���� �ý����� ������ ����ǰų�, ����� ���� �� ���带 �����ϴ� ��
	//���� ���������� ������ �̷�� ���� �ﰢ������ ó�����ش�
	_system->update();
}

void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter)
	{
		if (keyName == iter->first)
			return;
	}
	if (loop) //�Ϲ������� BGM
	{
		if (bgm)
		{
			//���� ������ �̸� ���ε��صΰ� ����Ѵ�
			//�Ѱ��� ���ϸ� �������
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
		else //���� ������
		{
			//���� ������ �׶��׶� �ǽð����� �ε��Ѵ�
			//���� ȿ������ ���� ũ�Ⱑ Ŭ��� ���۰� ���� ���� �ִ�
			//�������� ���� ������� �����ϱ⶧���� ȿ������ ����ϱ������ϴ�
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
	}
	else //�Ϲ������� ȿ����
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSound.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� ��Ƶд�
	_mTotalSound.insert(make_pair(keyName, &_sound[_mTotalSound.size()]));
}

void soundManager::play(string keyName, bool isBGM, float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			//��������
			if (isBGM)
				_channel[count]->setVolume(0);

			else
			{
				_channel[count]->setVolume( volume+_volumeSFX);
			}
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
			//���� ����
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
			//���� �Ͻ�����
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
			//���� �ٽ����
			_channel[count]->setPaused(false);
		}
	}
}

bool soundManager::isPlaySound(string keyName)
{
	bool isPlay = false;
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �Ͻ�����
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
			//���� �Ͻ�����
			_channel[count]->getPaused(&isPause);
			break;
		}
	}
	return isPause;
}

void soundManager::setBackGroundVolume(string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//��������
			_volumeBGM = volume;
			_channel[count]->setVolume(_volumeBGM);

		}
	}
}

void soundManager::fadeIn(string keyName,float volume)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setVolume(volume);
		}
	}
}
