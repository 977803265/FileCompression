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
	cout << "请输入要压缩的文件路径" << endl << ">>";
	gets(inputRoute);
	cout << "请输入要压缩的文件名" << endl << ">>";
	gets(inputFileName);
	cout << "请输入压缩后文件放置路径" << endl << ">>";
	gets(outputRoute);
	cout << "压缩中......" << endl;
	FileCompress input;
	input.Compress(inputRoute, outputRoute, inputFileName);
	cout << "压缩完成！！！！" << endl;
}


void UnCompress()
{
	getchar();
	char inputRoute[100] = { 0 };
	char outputRoute[100] = { 0 };
	char inputFileName[20] = { 0 };
	cout << "请输入要解压缩的文件路径" << endl << ">>";
	gets(inputRoute);
	cout << "请输入要解压缩的文件名" << endl << ">>";
	gets(inputFileName);
	cout << "请输入解压缩后文件输出路径" << endl << ">>";
	gets(outputRoute);
	cout << "解压缩中......" << endl;
	FileCompress output;
	if (output.UnCompress(inputRoute, outputRoute, inputFileName))
	cout << "解压缩完成！！！！" << endl;
}

void Select()
{
	while (1)
	{
		cout << "-------------请选择您需要的操作----------------" << endl;
		cout << "          1,压缩   2,解压   3,退出程序" << endl;
		cout << "请输入选项>>";
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