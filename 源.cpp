#include <iostream>
#include <assert.h>
using namespace std;

#pragma warning(disable:4018)
#include "FileCompression.h"


void Compress()
{
	getchar();
	char inputFile[100] = { 0 };
	cout << "������Ҫѹ�����ļ�·���Լ��ļ�����" << endl << ">>";
	gets(inputFile);
	cout << "ѹ����......" << endl;
	FileCompress input;
	input.Compress(inputFile);
	cout << "ѹ����ɣ�������" << endl;
}


void UnCompress()
{
	getchar();
	char outputFile[100] = { 0 };
	cout << "������Ҫ��ѹ�����ļ�·���Լ��ļ�����" << endl << ">>";
	gets(outputFile);
	cout << "��ѹ����......" << endl;
	FileCompress output;
	output.UnCompress(outputFile);
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