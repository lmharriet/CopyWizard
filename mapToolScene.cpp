#include "stdafx.h"
#include "mapToolScene.h"

HRESULT mapToolScene::init()
{
	IMAGEMANAGER->addImage("mapMenu", "maptool/ui/maptoolmenu.bmp", 360, 720);
	for (int i = 0; i < 3; i++)
	{
		option[i] = RectMake(1020, 197 + (i * 83), 160, 60);
	}
	return S_OK;
}

void mapToolScene::release()
{
}

void mapToolScene::update()
{
}

void mapToolScene::render()
{
	imageRender("mapMenu", { 920,0 });

	if (INPUT->GetToggleKey(VK_TAB))
	{
		for (int i = 0; i < 3; i++)
		{
			Rectangle(getMemDC(), option[i]);
		}
	}

	textRender();
}

void mapToolScene::imageRender(string keyName, POINT pt)
{
	IMAGEMANAGER->findImage(keyName)->render(getMemDC(), pt.x, pt.y);
}

void mapToolScene::textRender()
{
	ptOut(getMemDC(), { 10,30 }, _ptMouse);
}