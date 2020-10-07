#include "stdafx.h"
#include "image.h"
image::image() : _imageInfo(NULL), _fileName(NULL), _blendImage(NULL)
{
}
image::~image()
{
}

//�� ��Ʈ�� �ʱ�ȭ
HRESULT image::init(int width, int height)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �ִٸ� �������ؼ� �����ϰ� �ʱ�ȭ ����
	if (_imageInfo != NULL) this->release();

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����ü ���� ������ �ʱ�ȭ �ϱ�
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//�����̸�
	_fileName = NULL;
	//����Ű �÷� ����
	_isTrans = false;
	_transColor = RGB(0, 0, 0);

	//�̹��� ������ ��µ� �������� ��
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC �����ϱ�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//�̹��� ���Ϸ� �ʱ�ȭ
HRESULT image::init(const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �ִٸ� �������ؼ� �����ϰ� �ʱ�ȭ ����
	if (_imageInfo != NULL) this->release();

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����ü ���� ������ �ʱ�ȭ �ϱ�
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//�����̸�
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//����Ű �÷� ����
	_isTrans = isTrans;
	_transColor = transColor;

	//�̹��� ������ ��µ� �������� ��
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC �����ϱ�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
HRESULT image::init(const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �ִٸ� �������ؼ� �����ϰ� �ʱ�ȭ ����
	if (_imageInfo != NULL) this->release();

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����ü ���� ������ �ʱ�ȭ �ϱ�
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	//�����̸�
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//����Ű �÷� ����
	_isTrans = isTrans;
	_transColor = transColor;

	//�̹��� ������ ��µ� �������� ��
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC �����ϱ�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
HRESULT image::init(const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �ִٸ� �������ؼ� �����ϰ� �ʱ�ȭ ����
	if (_imageInfo != NULL) this->release();

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����ü ���� ������ �ʱ�ȭ �ϱ�
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;

	//�����̸�
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//����Ű �÷� ����
	_isTrans = isTrans;
	_transColor = transColor;

	//�̹��� ������ ��µ� �������� ��
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC �����ϱ�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
HRESULT image::init(const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �ִٸ� �������ؼ� �����ϰ� �ʱ�ȭ ����
	if (_imageInfo != NULL) this->release();

	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����ü ���� ������ �ʱ�ȭ �ϱ�
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;

	//�����̸�
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//����Ű �÷� ����
	_isTrans = isTrans;
	_transColor = transColor;

	//�̹��� ������ ��µ� �������� ��
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC �����ϱ�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
//���ĺ��� �ʱ�ȭ
HRESULT image::initForAlphaBlend()
{
	//DC ��������
	HDC hdc = GetDC(_hWnd);

	//���ĺ��� �ɼ� �ʱ�ȭ
	//BlendOp => ����� AC_SRC_OVER�� ��밡��
	//BlendFlags => �׳� 0����, ���� �� ��, ���� OS���� ����ߴٰ� ��
	//AlphaFormat => 24bit���� 0, 32bit���� AC_SRC_ALPHA ����ϸ� ������ �����̰� ��� �츰 �׳� 0 
	_blendFunc.BlendOp = AC_SRC_OVER;
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	//0 ~ 255 => 255�϶� ��������
	//_blendFunc.SourceConstantAlpha = 255;

	//���ĺ��� �̹��� ���� ����ü ���� ������ �ʱ�ȭ �ϱ�
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_FILE;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = WINSIZEX;
	_blendImage->height = WINSIZEY;

	//DC �����ϱ�
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::initForStretchBlend()
{
	HDC hdc = GetDC(_hWnd);

	_stretchImage = new IMAGE_INFO;
	_stretchImage->loadType = LOAD_FILE;
	_stretchImage->hMemDC = CreateCompatibleDC(hdc);
	_stretchImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	_stretchImage->hOBit = (HBITMAP)SelectObject(_stretchImage->hMemDC, _stretchImage->hBit);
	_stretchImage->width = WINSIZEX;
	_stretchImage->height = WINSIZEY;

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//����
void image::release()
{
	//�̹��� ������ ���� �ִٸ� ���� ���Ѷ�
	if (_imageInfo)
	{
		//�̹��� ����
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//������ ����
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE_ARRAY(_fileName);

		//���� �÷�Ű �ʱ�ȭ
		_isTrans = false;
		_transColor = RGB(0, 0, 0);
	}

	//���ĺ��� �̹��� ������ ���� �ִٸ� ���� ���Ѷ�
	if (_blendImage)
	{
		//�̹��� ����
		SelectObject(_blendImage->hMemDC, _blendImage->hOBit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);
		
		//������ ����
		SAFE_DELETE(_blendImage);
	}
}

//����(���� ���ϴ� ��ǥ�� ����ض�, �Ϲ������� �츮�� RECT�� LT��ǥ�� �̹����� ���δ�)
void image::render(HDC hdc, int destX, int destY)
{
	if (_isTrans)//���� ���ְ� ���
	{
		//GdiTransparentBlt : ��Ʈ�� �̹����� Ư�������� �����ϰ� ���� ���ִ� �Լ�
		GdiTransparentBlt(
			hdc,				//������ ����� DC
			destX,				//������ ��ǥ ����X
			destY,				//������ ��ǥ ����Y
			_imageInfo->width,	//������ �̹��� ����ũ��
			_imageInfo->height,	//������ �̹��� ����ũ��
			_imageInfo->hMemDC,	//����� ��� DC
			0, 0,				//����� ����� ��������
			_imageInfo->width,	//���� ���� ����ũ��
			_imageInfo->height,	//���� ���� ����ũ��
			_transColor);		//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		//BitBlt : DC���� �������� ��Ӻ��� ���ִ� �Լ�
		//�޸�DC => ȭ��DC�� �����Ѵ�
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY, int scale)
{
	if (_isTrans)//���� ���ְ� ���
	{
		//GdiTransparentBlt : ��Ʈ�� �̹����� Ư�������� �����ϰ� ���� ���ִ� �Լ�
		GdiTransparentBlt(
			hdc,				//������ ����� DC
			destX,				//������ ��ǥ ����X
			destY,				//������ ��ǥ ����Y
			_imageInfo->width,	//������ �̹��� ����ũ��
			_imageInfo->height,	//������ �̹��� ����ũ��
			_imageInfo->hMemDC,	//����� ��� DC
			0, 0,				//����� ����� ��������
			_imageInfo->width += scale,	//���� ���� ����ũ��
			_imageInfo->height += scale,	//���� ���� ����ũ��
			_transColor);		//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		//BitBlt : DC���� �������� ��Ӻ��� ���ִ� �Լ�
		//�޸�DC => ȭ��DC�� �����Ѵ�
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

//����(���ϴ� ��ǥ�� �̹����� �߶� ���̱�)
void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_isTrans)//���� ���ְ� ���
	{
		//GdiTransparentBlt : ��Ʈ�� �̹����� Ư�������� �����ϰ� ���� ���ִ� �Լ�
		GdiTransparentBlt(
			hdc,				//������ ����� DC
			destX,				//������ ��ǥ ����X
			destY,				//������ ��ǥ ����Y
			sourWidth,			//������ �̹��� ����ũ��
			sourHeight,			//������ �̹��� ����ũ��
			_imageInfo->hMemDC,	//����� ��� DC
			sourX, sourY,		//����� ����� ��������
			sourWidth,			//���� ���� ����ũ��
			sourHeight,			//���� ���� ����ũ��
			_transColor);		//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		//BitBlt : DC���� �������� ��Ӻ��� ���ִ� �Լ�
		//�޸�DC => ȭ��DC�� �����Ѵ�
		BitBlt(hdc, destX, destY, sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

void image::renderResize(HDC hdc, int destX, int destY, int sourWidth, int sourHeight, RECT rc, int defaultSize)
{
	float scale = (float)(rc.right - rc.left) / defaultSize;
	//
	if (_isTrans)//���� ���ְ� ���
	{
		GdiTransparentBlt(hdc, destX, destY, sourWidth * scale, sourHeight * scale,
			_imageInfo->hMemDC, 0, 0, sourWidth, sourHeight, _transColor);
	}
	else//���� �̹��� �״�� ���
	{
		//BitBlt : DC���� �������� ��Ӻ��� ���ִ� �Լ�
		//�޸�DC => ȭ��DC�� �����Ѵ�

		StretchBlt(hdc, destX, destY, _imageInfo->width * scale, _imageInfo->height * scale,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, SRCCOPY);
	}
}

void image::stretchRender(HDC hdc, int destX, int destY, float scale)
{
	if (!_stretchImage) this->initForStretchBlend();

	if (_isTrans) //���� ���ְ� ���
	{
		BitBlt(_stretchImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, 0, 0, SRCCOPY);

		// ���� ���̰ų� ���̰�
		StretchBlt(
			_stretchImage->hMemDC,
			0,
			0,
			_imageInfo->width * scale,
			_imageInfo->height * scale,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			SRCCOPY
		);

		// �� �ڿ� ����ȭ�Ѵ�
		GdiTransparentBlt(
			hdc,
			destX,
			destY,
			_imageInfo->width * scale,
			_imageInfo->height * scale,
			_stretchImage->hMemDC,
			0,
			0,
			_imageInfo->width * scale,
			_imageInfo->height * scale,
			_transColor);
	}
	else//���� �̹��� �״�� ���
	{
		StretchBlt(hdc, destX, destY, _imageInfo->width * scale, _imageInfo->height * scale,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, SRCCOPY);
	}
}

void image::stretchFrameRender(HDC hdc, int destX, int destY,int currentFrameX, int currentFrameY, float scale)
{
	//�̹��� ����ó��
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (!_stretchImage) this->initForStretchBlend();

	if (_isTrans) //���� ���ְ� ���
	{
		//BitBlt(_stretchImage->hMemDC, 0, 0, _imageInfo->frameWidth * scale, _imageInfo->frameHeight * scale,
		//	hdc, destX, destY, SRCCOPY);
		BitBlt(_stretchImage->hMemDC, 0, 0, _imageInfo->frameWidth * scale + destX, _imageInfo->frameHeight * scale + destY,
			hdc, 0, 0, SRCCOPY);

		// ���� ���̰ų� ���̰�
		StretchBlt(
			_stretchImage->hMemDC,
			0,
			0,
			_imageInfo->frameWidth * scale,
			_imageInfo->frameHeight * scale,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			SRCCOPY
		);
		/*
		* 	GdiTransparentBlt(
			hdc,						//������ ����� DC
			destX,						//������ ��ǥ ����X
			destY,						//������ ��ǥ ����Y
			_imageInfo->frameWidth,		//������ �̹��� ����ũ��
			_imageInfo->frameHeight,	//������ �̹��� ����ũ��
			_imageInfo->hMemDC,			//����� ��� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//����� ����� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//����� ����� ��������			
			_imageInfo->frameWidth,		//���� ���� ����ũ��
			_imageInfo->frameHeight,	//���� ���� ����ũ��
			_transColor);				//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
		*/

		// �� �ڿ� ����ȭ�Ѵ�
		GdiTransparentBlt(
			hdc,
			destX,
			destY,
			_imageInfo->frameWidth * scale,
			_imageInfo->frameHeight * scale,
			_stretchImage->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth * scale,
			_imageInfo->frameHeight * scale,
			_transColor);
	}
	else//���� �̹��� �״�� ���
	{
		StretchBlt(hdc, destX, destY, _imageInfo->width * scale, _imageInfo->height * scale,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, SRCCOPY);
	}
}


void image::alphaRender(HDC hdc, BYTE alpha)
{
	//���ĺ��� ����� �� �ֵ��� �ʱ�ȭ �ض�
	if (!_blendImage) this->initForAlphaBlend();
	//���İ� �ʱ�ȭ
	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//���� ���ְ� ���
	{
		//1. ����ȭ��DC�� ������ => �����̹����� ����
		//2. �̹����޸�DC�� ������ ���� �� => �����̹����� ����
		//3. �����̹����� ȭ��DC�� ����

		//1
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, 0, 0, SRCCOPY);

		//2
		//GdiTransparentBlt : ��Ʈ�� �̹����� Ư�������� �����ϰ� ���� ���ִ� �Լ�
		GdiTransparentBlt(
			_blendImage->hMemDC,	//������ ����� DC
			0,						//������ ��ǥ ����X
			0,						//������ ��ǥ ����Y
			_imageInfo->width,		//������ �̹��� ����ũ��
			_imageInfo->height,		//������ �̹��� ����ũ��
			_imageInfo->hMemDC,		//����� ��� DC
			0, 0,					//����� ����� ��������
			_imageInfo->width,		//���� ���� ����ũ��
			_imageInfo->height,		//���� ���� ����ũ��
			_transColor);			//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	
		//3
		GdiAlphaBlend(hdc, 0, 0, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else//���� �̹��� �״�� ���
	{
		GdiAlphaBlend(hdc, 0, 0, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{

	if (!_blendImage) this->initForAlphaBlend();
	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//���� ���ְ� ���
	{
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC,	//������ ����� DC
			0,						//������ ��ǥ ����X
			0,						//������ ��ǥ ����Y
			_imageInfo->width,		//������ �̹��� ����ũ��
			_imageInfo->height,		//������ �̹��� ����ũ��
			_imageInfo->hMemDC,		//����� ��� DC
			0, 0,					//����� ����� ��������
			_imageInfo->width,		//���� ���� ����ũ��
			_imageInfo->height,		//���� ���� ����ũ��
			_transColor);			//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)

		GdiAlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else//���� �̹��� �״�� ���
	{
		GdiAlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	if (!_blendImage) this->initForAlphaBlend();
	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//���� ���ְ� ���
	{
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC,	//������ ����� DC
			0,						//������ ��ǥ ����X
			0,						//������ ��ǥ ����Y
			sourWidth,				//������ �̹��� ����ũ��
			sourHeight,				//������ �̹��� ����ũ��
			_imageInfo->hMemDC,		//����� ��� DC
			sourX, sourY,			//����� ����� ��������
			sourWidth,				//���� ���� ����ũ��
			sourHeight,				//���� ���� ����ũ��
			_transColor);			//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)

		GdiAlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
	else//���� �̹��� �״�� ���
	{
		GdiAlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, _blendFunc);
	}
}

void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_isTrans)//���� ���ְ� ���
	{
		GdiTransparentBlt(
			hdc,						//������ ����� DC
			destX,						//������ ��ǥ ����X
			destY,						//������ ��ǥ ����Y
			_imageInfo->frameWidth,		//������ �̹��� ����ũ��
			_imageInfo->frameHeight,	//������ �̹��� ����ũ��
			_imageInfo->hMemDC,			//����� ��� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//����� ����� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//����� ����� ��������			
			_imageInfo->frameWidth,		//���� ���� ����ũ��
			_imageInfo->frameHeight,	//���� ���� ����ũ��
			_transColor);				//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC, 
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	//�̹��� ����ó��
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (_isTrans)//���� ���ְ� ���
	{
		GdiTransparentBlt(
			hdc,						//������ ����� DC
			destX,						//������ ��ǥ ����X
			destY,						//������ ��ǥ ����Y
			_imageInfo->frameWidth,		//������ �̹��� ����ũ��
			_imageInfo->frameHeight,	//������ �̹��� ����ũ��
			_imageInfo->hMemDC,			//����� ��� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//����� ����� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//����� ����� ��������			
			_imageInfo->frameWidth,		//���� ���� ����ũ��
			_imageInfo->frameHeight,	//���� ���� ����ũ��
			_transColor);				//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}



void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, RECT rc, int defaultSize)
{
	float scale = (float)(rc.right - rc.left) / defaultSize;
	//�̹��� ����ó��
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (_isTrans)//���� ���ְ� ���
	{
		GdiTransparentBlt(
			hdc,						//������ ����� DC
			destX,						//������ ��ǥ ����X
			destY,						//������ ��ǥ ����Y
			_imageInfo->frameWidth * scale,		//������ �̹��� ����ũ��
			_imageInfo->frameHeight * scale,	//������ �̹��� ����ũ��
			_imageInfo->hMemDC,			//����� ��� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//����� ����� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//����� ����� ��������			
			_imageInfo->frameWidth,		//���� ���� ����ũ��
			_imageInfo->frameHeight,	//���� ���� ����ũ��
			_transColor);				//�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, RECT scale)
{
	//if (!_stretchImage) this->initForStretchBlend();

	//�̹��� ����ó��
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (_isTrans)//���� ���ְ� ���
	{
		GdiTransparentBlt(
			hdc,                        //������ ����� DC
			destX,                        //������ ��ǥ ����X
			destY,                        //������ ��ǥ ����Y
			scale.right - scale.left,    //������ �̹��� ����ũ��
			scale.right - scale.left,    //������ �̹��� ����ũ��
			_imageInfo->hMemDC,            //����� ��� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,     //����� ����� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,    //����� ����� ��������
			_imageInfo->frameWidth,        //���� ���� ����ũ��
			_imageInfo->frameHeight,    //���� ���� ����ũ��
			_transColor);                //�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, float scale)
{
	//if (!_stretchImage) this->initForStretchBlend();

	//�̹��� ����ó��
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (_isTrans)//���� ���ְ� ���
	{
		GdiTransparentBlt(
			hdc,                        //������ ����� DC
			destX,                        //������ ��ǥ ����X
			destY,                        //������ ��ǥ ����Y
			_imageInfo->frameWidth * scale,    //������ �̹��� ����ũ��
			_imageInfo->frameHeight * scale,    //������ �̹��� ����ũ��
			_imageInfo->hMemDC,            //����� ��� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,     //����� ����� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,    //����� ����� ��������
			_imageInfo->frameWidth,        //���� ���� ����ũ��
			_imageInfo->frameHeight,    //���� ���� ����ũ��
			_transColor);                //�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)
	}
	else//���� �̹��� �״�� ���
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

//���� �����ӷ���
void image::alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	//�̹��� ����ó��
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (!_blendImage) this->initForAlphaBlend();

	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//���� ���ְ� ���
	{
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC,        //������ ����� DC
			0,                            //������ ��ǥ ����X
			0,                            //������ ��ǥ ����Y
			_imageInfo->frameWidth,        //������ �̹��� ����ũ��
			_imageInfo->frameHeight,    //������ �̹��� ����ũ��
			_imageInfo->hMemDC,            //����� ��� DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,                            //����� ����� ��������
			_imageInfo->currentFrameY * _imageInfo->frameHeight,                            //����� ����� ��������
			_imageInfo->frameWidth,        //���� ���� ����ũ��
			_imageInfo->frameHeight,    //���� ���� ����ũ��
			_transColor);                //�����Ҷ� ������ ���� (�Ϲ������� ����Ÿ ������ �����)


		GdiAlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);
	}
	else//���� �̹��� �״�� ���
	{
		//���� ��������
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}


//��������
void image::loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	//������(offset)�� �Ϲ������� ���� ������Ʈ �ȿ��� ������Ʈ ó������ �־��� ��ҳ� Ư�� ���������� ������
		//offset�� � Ư���� ������ �ణ ���̰� ���� �� �Ǵ� �� ���� ��ü�� �ǹ�(���̿� ���� ������)

		//offset ���� ������ ��� �����ϱ�
	if (offsetX < 0) offsetX = _imageInfo->width + (offsetX % _imageInfo->width);
	if (offsetY < 0) offsetY = _imageInfo->height + (offsetY % _imageInfo->height);

	//�׷����� ���� ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//�׷����� DC ���� (ȭ��ũ��)
	RECT rcDest;

	//�׷��� �� ��ü ����
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//���� ��������
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//�ҽ� ������ ���� ���
		rcSour.top = (y + offsetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//�ҽ� ������ �׸��� ȭ���� �Ѿ�ٸ�(ȭ������� ��������)
		if (y + sourHeight > drawAreaH)
		{
			//�Ѿ �׸��� ����ŭ ���Ұ��� �÷��ش�
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//�׷����� ����
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//���� ��������
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//�ҽ� ������ ���� ���
			rcSour.left = (x + offsetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			//�ҽ� ������ �׸��� ȭ���� �Ѿ�ٸ�(ȭ������� ��������)
			if (x + sourWidth > drawAreaW)
			{
				//�Ѿ �׸��� ����ŭ ���Ұ��� �÷��ش�
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//�׷����� ����
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//�׷�����(�Ϲݷ���-�̹����߶󼭺��̱�)
			render(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top, sourWidth, sourHeight);
		}
	}
}

void image::loopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha)
{
	//offset ���� ������ ��� �����ϱ�
	if (offsetX < 0) offsetX = _imageInfo->width + (offsetX % _imageInfo->width);
	if (offsetY < 0) offsetY = _imageInfo->height + (offsetY % _imageInfo->height);

	//�׷����� ���� ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//�׷����� DC ���� (ȭ��ũ��)
	RECT rcDest;

	//�׷��� �� ��ü ����
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//���� ��������
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//�ҽ� ������ ���� ���
		rcSour.top = (y + offsetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//�ҽ� ������ �׸��� ȭ���� �Ѿ�ٸ�(ȭ������� ��������)
		if (y + sourHeight > drawAreaH)
		{
			//�Ѿ �׸��� ����ŭ ���Ұ��� �÷��ش�
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//�׷����� ����
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//���� ��������
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//�ҽ� ������ ���� ���
			rcSour.left = (x + offsetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			//�ҽ� ������ �׸��� ȭ���� �Ѿ�ٸ�(ȭ������� ��������)
			if (x + sourWidth > drawAreaW)
			{
				//�Ѿ �׸��� ����ŭ ���Ұ��� �÷��ش�
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//�׷����� ����
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//�׷�����(���ķ���-�̹����߶󼭺��̱�)
			alphaRender(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top, sourWidth, sourHeight, alpha);
		}
	}
}

void image::frameStretchAlphaRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, float scaleX, float scaleY, BYTE alpha)
{
	//�̹��� ����ó��
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (!_stretchImage) this->initForStretchBlend();
	if (!_blendImage) this->initForAlphaBlend();
	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans) //���� ���ְ� ���
	{
		BitBlt(_stretchImage->hMemDC, 0, 0, _imageInfo->width * scaleX + destX, _imageInfo->height * scaleY + destY,
			hdc, 0, 0, SRCCOPY);
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width * scaleX + destX, _imageInfo->height * scaleY + destY,
			hdc, 0, 0, SRCCOPY);

		// ���� ���̰ų� ���̰�
		StretchBlt(
			_stretchImage->hMemDC,
			destX,
			destY,
			_imageInfo->width * scaleX,
			_imageInfo->height * scaleY,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			SRCCOPY
		);

		// �� �ڿ� ����ȭ�Ѵ�
		GdiTransparentBlt(
			_blendImage->hMemDC,
			destX,
			destY,
			_imageInfo->frameWidth * scaleX,
			_imageInfo->frameHeight * scaleY,
			_stretchImage->hMemDC,
			(_imageInfo->frameWidth * currentFrameX * scaleX) + destX,
			(_imageInfo->frameHeight * currentFrameY * scaleY) + destY,
			_imageInfo->frameWidth * scaleX,
			_imageInfo->frameHeight * scaleY,
			_transColor);

		GdiAlphaBlend(hdc, destX, destY, _imageInfo->frameWidth * scaleX, _imageInfo->frameHeight * scaleY,
			_blendImage->hMemDC, destX + (_imageInfo->frameWidth * currentFrameX * scaleX), destY + (_imageInfo->frameHeight * currentFrameY * scaleY),
			_imageInfo->frameWidth * scaleX, _imageInfo->frameHeight * scaleY, _blendFunc);
	}
	else // Ȯ��� ���ĸ� ���� �� (����ȭ�� ����)
	{
		StretchBlt(_blendImage->hMemDC, destX, destY, _imageInfo->frameWidth * scaleX, _imageInfo->frameHeight * scaleY,
			_imageInfo->hMemDC, currentFrameX * _imageInfo->frameWidth * scaleX, currentFrameY * _imageInfo->frameHeight * scaleY, _imageInfo->frameWidth * scaleX, _imageInfo->frameHeight * scaleY, SRCCOPY);

		GdiAlphaBlend(hdc, destX, destY, _imageInfo->frameWidth * scaleX, _imageInfo->frameHeight * scaleY,
			_blendImage->hMemDC, destX + (currentFrameX * _imageInfo->frameWidth * scaleX), destY + (currentFrameY * _imageInfo->frameHeight * scaleY),
			_imageInfo->frameWidth * scaleX, _imageInfo->frameHeight * scaleY, _blendFunc);
	}
}