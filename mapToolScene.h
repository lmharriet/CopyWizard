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

//struct tagButton
//{
//	RECT rc;
//};

class mapToolScene : public gameNode
{
private:
	RECT mapOption[3];
	
	/// BUTTON ///
	RECT SAVE, LOAD, BACK;

	OPTION option;
	//vk_down
	bool isLeftDown, isLeft, isLeftUp;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void imageRender(string keyName, POINT pt);

	void textRender();
	//don't touch
	void buttonRender();
	void vkCheck();
};