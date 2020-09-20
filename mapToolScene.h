#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"

class mapToolScene : public gameNode
{
private:
	/// USER ///
	tagUser user;
	TOOL tool;

	/// TILE ///
	tagTile tile[MAXTILE];

	/// OPTION ///
	RECT mapOption[3];
	OPTION option;
	
	/// BUTTON ///
	RECT SAVE, LOAD, BACK;
	///  ICON  ///
	RECT icon[4];
	tagIcon drag;

	tagTile bigTile[4];
	tagTile miniTile[4];

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

	//reset
	void resetUserData();

	//init
	void initTile();
	void initSelectTile();
	void initCam();
	//camera
	void moveRect();

	//image
	void addImage();

	//render
	void tileRender();
	void UIRender();
	void textRender();
	void buttonRender();
	void rcRender();
	void checkRender();

	void imageRender(string keyName, POINT pt);
	void imageAlphaRender(string keyName, POINT pt, int alpha = 150);
	void imageFrameRender(string keyName, POINT pt, int frameX, int frameY);

	//don't touch
	void controller();
	void vkCheck();
};