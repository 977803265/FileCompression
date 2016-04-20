#include <iostream>
#include <assert.h>
using namespace std;

#pragma warning(disable:4018)
#include "FileCompression.h"


void Compress()
{
	getchar();
	char inputFile[100] = { 0 };
	cout << "请输入要压缩的文件路径以及文件名称" << endl << ">>";
	gets(inputFile);
	cout << "压缩中......" << endl;
	FileCompress input;
	input.Compress(inputFile);
	cout << "压缩完成！！！！" << endl;
}


void UnCompress()
{
	getchar();
	char outputFile[100] = { 0 };
	cout << "请输入要解压缩的文件路径以及文件名称" << endl << ">>";
	gets(outputFile);
	cout << "解压缩中......" << endl;
	FileCompress output;
	output.UnCompress(outputFile);
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