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

	//BGM_Sound
	SOUNDMANAGER->addSound("titleBGM", "Sound/TitleScreen.mp3", true, true);
	SOUNDMANAGER->addSound("mapToolBGM", "Sound/MapToolBGM.mp3", true, true);
	volume = 0.5f;
	
	


	/*���߰�*/
	SCENEMANAGER->addScene("����ȭ��", new startScene);
	SCENEMANAGER->addScene("INI�׽�Ʈ", new iniTestScene);
	//SCENEMANAGER->addScene("����", new soundTestScene);
	SCENEMANAGER->addScene("�ȼ��浹", new pixelCollisionScene);
	SCENEMANAGER->addScene("�ε�ȭ��", new loadingScene);

	mapToolScene* maptool = new mapToolScene;
	SCENEMANAGER->addScene("��������", maptool);
	maptool->init();
	maptool->addImage();
	/*�����*/
	SCENEMANAGER->loadScene("����ȭ��");

	ShowCursor(false);
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
	gameNode::update();

	//���� ��Ʈ�� 
	this->volumeControl();
	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();

	//����Ŵ��� ������Ʈ (�̰� ������ ����Ŵ��� ����� �������� �ʴ´�!!!)
	SOUNDMANAGER->update();

	
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
		}
	}
	if (INPUT->GetKeyDown(VK_SUBTRACT) || INPUT->GetKeyDown(VK_OEM_MINUS))
	{
		if (volume > .0f)
		{
			volume -= .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
		}
	}
}
