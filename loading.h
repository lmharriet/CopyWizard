#pragma once
#include "gameNode.h"
#include "progressBar.h"

//=============================================================
//	## loadItem ## (�ε������ - �̹�������, ��������)
//=============================================================

//�ε������ ����
enum LOAD_KIND
{
	LOAD_KIND_IMAGE_0,			//���Ʈ�� �̹���
	LOAD_KIND_IMAGE_1,			//�Ϲ� �̹���
	LOAD_KIND_IMAGE_2,			//�Ϲ� �̹���(��ǥ)
	LOAD_KIND_FRAMEIMAGE_0,		//������ �̹���
	LOAD_KIND_FRAMEIMAGE_1,		//������ �̹���(��ǥ)
	LOAD_KIND_SOUND,			//����
	LOAD_KIND_END
};

//�̹��� ���ҽ� ����ü
struct tagImageResource
{
	string keyName;				//�̹��� Ű��
	const char* fileName;		//�̹��� �����̸�
	int x, y;					//��ġ��ǥ
	int width, height;			//����, ���α���
	int frameX, frameY;			//������ x, y
	bool isTrans;				//����������?
	COLORREF transColor;		//�������
};

//���� ���ҽ� ����ü
struct tagSoundResource
{
	string keyName;				//���� Ű��
	const char* fileName;		//���� �����̸�
	bool bgm;					//����?
	bool loop;					//����?
};

class loadItem
{
private:
	LOAD_KIND _kind;					//�ε�����
	tagImageResource _imageResource;	//�̹��� ����

	tagSoundResource _soundResource;	//���� ����
public:
	//image
	HRESULT init(string strKey, int width, int height);
	HRESULT init(string strKey, const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(string strKey, const char* fileName, float x, float y, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	HRESULT init(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	//sound
	HRESULT init(string strKey, const char* fileName, bool bgm = false, bool loop = false);

	//�ε������� ���� ��������
	LOAD_KIND getLoadKind() { return _kind; }
	//�̹��� ���ҽ� ��������
	tagImageResource getImageResource() { return _imageResource; }
	//���� ���ҽ� ��������
	tagSoundResource getSoundResource() { return _soundResource; }

	string getImageName_key() { return _imageResource.keyName; }
	const char* getImageName() { return _imageResource.fileName; }
};

//=============================================================
//	## loading ## (�ε�Ŭ���� - �ε�ȭ�� �����ϱ�, �ε���, ��׶���)
//=============================================================
class loading : public gameNode
{
private:
	//�ε������ Ŭ������ ���� ���� �� �ݺ���
	typedef vector<loadItem*> vLoadItem;
	typedef vector<loadItem*>::iterator viLoadItem;

private:
	vLoadItem _vLoadItem;

	//�̺κ��� �ε�ȭ�鿡�� ����� �̹����� �ε���
	image* _background;
	progressBar* _loadingBar;
	int _currentGauge;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void loadImage(string strKey, int width, int height);
	void loadImage(string strKey, const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	void loadImage(string strKey, const char* fileName, float x, float y, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	void loadFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	void loadFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));

	void loadSound(string strKey, const char* fileName, bool bgm = false, bool loop = false);
	/*�߿���*/
	//�ε��Ϸ� �Ƴ�? (�ε��Ϸ��� ȭ����ȯ)
	bool loadingDone();

	//�ε������ Ŭ���� ����ִ� ���� ��������
	vLoadItem getLoadItem() { return _vLoadItem; }
	//��������� ��������
	int getCurrentGauge() { return _currentGauge; }
};