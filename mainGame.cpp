#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	IMAGEMANAGER->addImage("cursor", "wizard/cursor.bmp", 64, 64, true, RGB(255, 0, 255));

	/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
	/*�̰����� ������ ������ �߰��ϰ� ������� �����Ѵ�*/

	soundInit();

	
	/*���߰�*/

	SCENEMANAGER->addScene("�ε�ȭ��", make_shared<loadingScene>());
	SCENEMANAGER->addScene("����ȭ��", make_shared <startScene>());
	SCENEMANAGER->addScene("�ΰ���", make_shared<gameScene>());
	SCENEMANAGER->addScene("������", make_shared<bossScene>());
	SCENEMANAGER->addScene("����������", make_shared<finalBossScene>());

	shared_ptr<mapToolScene> maptool = make_shared <mapToolScene>();
	SCENEMANAGER->addScene("��������", maptool);
	maptool->init();
	maptool->addImage();
	/*�����*/

	//SCENEMANAGER->loadScene("�ΰ���");
	SCENEMANAGER->loadScene("����ȭ��");
	//SCENEMANAGER->loadScene("��������");
	//SCENEMANAGER->loadScene("������");
	//SCENEMANAGER->loadScene("����������");

	ShowCursor(false);
	time = 0;
	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	
	gameNode::release();
}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void mainGame::update()
{
	//gameNode::update();
	time++;
	//if (INPUT->GetKeyDown(VK_SPACE))TIME->setTest(12.f);

	//���� ��Ʈ�� 
	this->volumeControl();
	//���Ŵ��� ������Ʈ
	if (time % (int)TIME->getTest() == 0)
	{
		gameNode::update();
		SCENEMANAGER->update();
	}

	//����Ŵ��� ������Ʈ (�̰� ������ ����Ŵ��� ����� �������� �ʴ´�!!!)
	SOUNDMANAGER->update();

	if(INPUT->GetKeyDown(0x31)) // 1
		SCENEMANAGER->loadScene("����ȭ��");
	if(INPUT->GetKeyDown(0x32)) // 2
		SCENEMANAGER->loadScene("��������");
	if(INPUT->GetKeyDown(0x33)) // 3
		SCENEMANAGER->loadScene("�ΰ���");
	if(INPUT->GetKeyDown(0x34)) // 4
		SCENEMANAGER->loadScene("������");


	//if (INPUT->GetKey(VK_SPACE) && (INT)TIME->getTest() > 1)
	//{
	//	TIME->setTest(-TIME->Lerp(0, TIME->getTest(), TIME->deltaTime()));
	//	cout << (INT)TIME->getTest() << '\n';
	//	//TIME->Lerp()
	//}
}

//=============================================================
//	## ���� ## render()
//=============================================================
void mainGame::render()
{
	//��� �� ��Ʈ�� (�̰��� ������ �׳� �α�)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	
	//���ڻ� ��� ���ֱ�
	SetBkMode(getMemDC(), TRANSPARENT);

	//���Ŵ��� ����
	SCENEMANAGER->render();
	TIME->render(getMemDC());

	IMAGEMANAGER->findImage("cursor")->render(getMemDC(), _ptMouse.x - 32, _ptMouse.y - 32);

	
//=============================================================
	//������� ������ ȭ��DC�� �׸��� (�̰͵� ������ �׳� �α�)
	this->getBackBuffer()->render(getHDC());
}

void mainGame::volumeControl()
{
	if (INPUT->GetKeyDown(VK_ADD) || INPUT->GetKeyDown(VK_OEM_PLUS))
	{
		if (volume < 1.f)
		{
			volume += .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
		}
	}
	if (INPUT->GetKeyDown(VK_SUBTRACT) || INPUT->GetKeyDown(VK_OEM_MINUS))
	{
		if (volume > .0f)
		{
			volume -= .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
		}
	}

	if (INPUT->GetKeyDown(VK_OEM_4))//down
	{
		if (sfxVolume > -1.0f)
			sfxVolume -= 0.05f;
		SOUNDMANAGER->setVolumeSFX(sfxVolume);
	}
	if(INPUT->GetKeyDown(VK_OEM_6))//up
	{
		if (sfxVolume < 1.0f)
			sfxVolume += 0.05f;
		SOUNDMANAGER->setVolumeSFX(sfxVolume);
	}

	if (INPUT->GetKeyDown(VK_OEM_3)) //mute
	{
		if (!isMute)
		{
			SOUNDMANAGER->setVolumeSFX(-1.0f);
			SOUNDMANAGER->setBackGroundVolume("titleBGM", -1.0f);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", -1.0f);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", -1.0f);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", -1.0f);
			isMute = true;
		}
		else
		{
			SOUNDMANAGER->setVolumeSFX(sfxVolume);
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
			isMute = false;
		}

	}
	
}

void mainGame::soundInit()
{
	
	//BGM_Sound
	SOUNDMANAGER->addSound("titleBGM", "Sound/TitleScreen.mp3", true, true);
	SOUNDMANAGER->addSound("mapToolBGM", "Sound/MapToolBGM.mp3", true, true);
	SOUNDMANAGER->addSound("ingameBGM", "Sound/IngameBGM.mp3", true, true);
	SOUNDMANAGER->addSound("bossBGM", "Sound/BossBGM.mp3", true, true);
	
	volume = 0.5f;
	
	
	

	sfxVolume = 0.5f;
	isMute = false;
}
