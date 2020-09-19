#pragma once
#include "singletonBase.h"
//=============================================================
//	## iniData ## (INI������)
//=============================================================

//INI������ ����ü
struct tagIniData
{
	char* section;  //[Player]
	char* key;		//HP=100
	char* value;
};


class iniData : public singletonBase <iniData>
{
private:
	vector<tagIniData> _vIniData;

public:
	HRESULT init();
	void release();

	//������ �߰��ϱ�
	void addData(const char* section, const char* key, const char* value);
	//������ ���̺��ϱ�
	void saveINI(const char* fileName);

	//���ڷ� ���� �ε��ϱ�(�����̸�, ����, Ű)
	char* loadDataString(const char* fileName, const char* section, const char* key);
	//������ ���� �ε��ϱ�(�����̸�, ����, Ű)
	int loadDataInteger(const char* fileName, const char* section, const char* key);
	//�Ǽ��� ���� �ε��ϱ�(�����̸�, ����, Ű)
    float loadDataFloat(const char* fileName, const char* section, const char* key);

};

