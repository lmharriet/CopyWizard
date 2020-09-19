#include "stdafx.h"
#include "iniData.h"

HRESULT iniData::init()
{
	return S_OK;
}

void iniData::release()
{
}

//������ �߰��ϱ�
void iniData::addData(const char * section, const char * key, const char * value)
{
	//ini������ ����ü �ʱ�ȭ
	tagIniData data;
	data.section = (char*)section;
	data.key = (char*)key;
	data.value = (char*)value;

	//���Ϳ� ���
	_vIniData.push_back(data);
}

//������ ���̺��ϱ�
void iniData::saveINI(const char * fileName)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	//char test[256] = { 0 }; //�̷��� �ʱ�ȭ �ϴ� ����� ����
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	//D:\�ù� 22��\20200908_0_INI������\WindowAPI
	strcat(dir, str); //���ڿ� ���� D:\�ù� 22��\20200908_0_INI������\WindowAPI\fileName.ini

	for (int i = 0; i < _vIniData.size(); i++)
	{
		WritePrivateProfileString(_vIniData[i].section, _vIniData[i].key, _vIniData[i].value, dir);
	}
}

//���ڷ� ���� �ε��ϱ�(�����̸�, ����, Ű)
char * iniData::loadDataString(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	//char test[256] = { 0 }; //�̷��� �ʱ�ȭ �ϴ� ����� ����
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	//D:\�ù� 22��\20200908_0_INI������\WindowAPI
	strcat(dir, str); //���ڿ� ���� D:\�ù� 22��\20200908_0_INI������\WindowAPI\fileName.ini

	char data[64] = { 0 };

	GetPrivateProfileString(section, key, "", data, 64, dir);

	return data;
}

//������ ���� �ε��ϱ�(�����̸�, ����, Ű)
int iniData::loadDataInteger(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	//char test[256] = { 0 }; //�̷��� �ʱ�ȭ �ϴ� ����� ����
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	//D:\�ù� 22��\20200908_0_INI������\WindowAPI
	strcat(dir, str); //���ڿ� ���� D:\�ù� 22��\20200908_0_INI������\WindowAPI\fileName.ini

	return GetPrivateProfileInt(section, key, 0, dir);
}

//�Ǽ��� ���� �ε��ϱ�(�����̸�, ����, Ű)
float iniData::loadDataFloat(const char * fileName, const char * section, const char * key)
{
	return (atof)(loadDataString(fileName, section, key));
	//atoi => ���ڿ��� ������
	//atof => ���ڿ��� �Ǽ���
}
