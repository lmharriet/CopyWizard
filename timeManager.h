#pragma once
#include "singletonBase.h"
//==================================================================
//		## timeManager ## (Ÿ�� �Ŵ���)
//==================================================================

class timeManager : public singletonBase <timeManager>
{
private:
	bool _isHardware;				//���� Ÿ�̸Ӹ� �����ϳ�?
	float _timeScale;				//�ð� ���е� (1�ʿ� ����̳� ī������ �������� ����)
	float _deltaTime;				//�������Ӵ� �����
	__int64 _curTime;				//����ð�
	__int64 _lastTime;				//�������ð�
	__int64 _periodFrequency;		//�ð��ֱ� (1�ʿ� ����̳� ī��Ʈ�Ǵ���)

	unsigned long _frameRate;		//FPS
	unsigned long _FPSFrameCount;	//FPS ī��Ʈ
	float _FPSTimeElapsed;			//FPS �����
	float _time;						//��ü ����ð�

public:
	HRESULT init(void);
	void release(void);
	void update(float lockFPS);
	void render(HDC hdc);

	//�������Ӵ� ����ð� �������� (����Ƽ���� Time.deltaTime)
	float deltaTime(void) { return _deltaTime; }
	//��ü ����ð� �������� (����Ƽ���� Time.time)
	float time(void) { return _time; }

};
