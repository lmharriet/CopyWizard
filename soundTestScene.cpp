#include "stdafx.h"
#include "soundTestScene.h"


HRESULT soundTestScene::init()
{
	//���� �߰�
	
	//BGM�ϰ��
	//SOUNDMANAGER->addSound("����1", "Sound/���.mp3", true, true);
	
	//ȿ�����ϰ��
	SOUNDMANAGER->addSound("����1", "Sound/���.mp3");
	SOUNDMANAGER->addSound("����2", "Sound/���1.mp3");

	//�������ڸ��� �÷����ϱ�
	//SOUNDMANAGER->play("����1");


	return S_OK;
}

void soundTestScene::release()
{
}

void soundTestScene::update()
{
	//ȿ������ �׳� �浹�ɶ� �÷��̸� �ϸ� �ȴ�
	//����� ���� �÷��������� üũ�ؼ� ������ �÷��̸� �ؾ� �Ѵ�
	//�׷��� ������� �����뷡ó�� ���尡 ������ �ȴ�
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		if (SOUNDMANAGER->isPlaySound("����1"))
		{
			SOUNDMANAGER->stop("����1");
		}
		SOUNDMANAGER->play("����2");
	}
}

void soundTestScene::render()
{
	//��ũ����ǿ� �߰��ص� (�ؽ�Ʈ ��� ���ϰ� ����ϱ�)
	textOut(getMemDC(), 10, 10, "�����׽�Ʈ");
}
