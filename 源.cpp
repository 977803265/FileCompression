#include <iostream>
#include <assert.h>
using namespace std;

#pragma warning(disable:4018)
#include "FileCompression.h"


void Compress()
{
	getchar();
	char inputRoute[100] = { 0 };
	char outputRoute[100] = { 0 };
	char inputFileName[20] = { 0 };
	cout << "������Ҫѹ�����ļ�·��" << endl << ">>";
	gets(inputRoute);
	cout << "������Ҫѹ�����ļ���" << endl << ">>";
	gets(inputFileName);
	cout << "������ѹ�����ļ�����·��" << endl << ">>";
	gets(outputRoute);
	cout << "ѹ����......" << endl;
	FileCompress input;
	input.Compress(inputRoute, outputRoute, inputFileName);
	cout << "ѹ����ɣ�������" << endl;
}


void UnCompress()
{
	getchar();
	char inputRoute[100] = { 0 };
	char outputRoute[100] = { 0 };
	char inputFileName[20] = { 0 };
	cout << "������Ҫ��ѹ�����ļ�·��" << endl << ">>";
	gets(inputRoute);
	cout << "������Ҫ��ѹ�����ļ���" << endl << ">>";
	gets(inputFileName);
	cout << "�������ѹ�����ļ����·��" << endl << ">>";
	gets(outputRoute);
	cout << "��ѹ����......" << endl;
	FileCompress output;
	if (output.UnCompress(inputRoute, outputRoute, inputFileName))
	cout << "��ѹ����ɣ�������" << endl;
}

void Select()
{
	while (1)
	{
		cout << "-------------��ѡ������Ҫ�Ĳ���----------------" << endl;
		cout << "          1,ѹ��   2,��ѹ   3,�˳�����" << endl;
		cout << "������ѡ��>>";
		int select;
		cin >> select;
		switch (select)
		{
		case 1:Compress(); break;
		case 2:UnCompress(); break;
		case 3:exit(0); break;
		default:break;
		}
	}
	
}



int main()
{
	Select();
	return 0;
}