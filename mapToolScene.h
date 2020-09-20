#pragma once
#include "gameNode.h"
#include "tileNode.h"

enum class OPTION
{
	WALL,
	TILE,
	OBJECT,

	MENU
};

struct tagIcon
{
	RECT rc;
	bool isCol;
};

class mapToolScene : public gameNode
{
private:
	RECT mapOption[3];
	
	/// BUTTON ///
	RECT SAVE, LOAD, BACK;

	tagIcon icon[4];
	tagIcon drag;

	OPTION option;
	//vk_down
	bool isLeftDown, isLeft, isLeftUp;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void buttonCheck();
	void iconCheck();

	//image
	void addImage();

	//render
	void UIRender();
	void textRender();
	void buttonRender();
	void rcRender();
	void imageRender(string keyName, POINT pt);
	//don't touch
	void vkCheck();
};