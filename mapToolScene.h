#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"

class mapToolScene : public gameNode
{
private:

	tagRC maptool;
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
	tagRC drag;

	tagTile bigTile[4];
	//tagTile miniTile[4];
	
	tagTile wall[6];

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
	void initSelectTerrain();
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