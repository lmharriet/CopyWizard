#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"

class mapToolScene : public gameNode
{
private:
	/// TILE ///
	tagTile tile[MAXTILE];

	/// OPTION ///
	RECT mapOption[3];
	OPTION option;
	
	/// BUTTON ///
	RECT SAVE, LOAD, BACK;
	///  ICON  ///
	tagIcon icon[4];
	tagIcon drag;

	/// CAMERA ///
	tagCam cam;

	//vk_down
	bool isLeftDown, isLeft, isLeftUp;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void buttonCheck();
	void iconCheck();

	//init
	void initTile();
	void initCam();

	//camera
	void moveRect();

	//image
	void addImage();

	//render
	void UIRender();
	void textRender();
	void buttonRender();
	void rcRender();
	void imageRender(string keyName, POINT pt);

	//don't touch
	void controller();
	void vkCheck();
};