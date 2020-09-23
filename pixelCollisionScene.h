#pragma once
#include "gameNode.h"
#include "tileNode.h"

class pixelCollisionScene : public gameNode
{
private:
	//image* _mountain;		//��׶��� �̹���
	image* _ball;			//���̹���
	RECT _rc;				//����Ʈ
	float _x, _y;			//��ġ��ǥ
	int _probeY;			//�ȼ��浹 Ž���� Y��

	//��Ÿ�� ����
	tagTile tile[MAXTILE];

	//test cam(�����ص���)
	RECT cam;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//�� �ε�
	void loadMap(const char* mapFileName);
};