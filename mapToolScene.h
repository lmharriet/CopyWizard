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

	tagIcon icon[5];

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
	void imageRender(string keyName, POINT pt);
	void textRender();
	void buttonRender();
	//don't touch
	void vkCheck();
};