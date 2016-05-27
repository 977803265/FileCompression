//在写这份代码的时候，让我明白一件事情，永远没有最好的，永远不要对自己满足
//加油，coder
#pragma once 
#pragma warning(disable:4996)
#include"HuffmanTree.h"
#include<string.h>
#include<algorithm>

typedef  long long LongType;

//配置文件结构
struct FileInfo{
	char _ch;       //字符
	LongType _count; //字符在文件中出现次数（权值）
	string _code;    //对应哈夫曼码

	FileInfo(const unsigned char ch = 0)
		:_ch(ch)
		,_count(0)
	{}

	bool operator < (const FileInfo &info)
	{
		return this->_count < info._count;
	}

	FileInfo operator + (const FileInfo &info)
	{
		FileInfo tmp;
		tmp._count = this->_count + info._count;
		return tmp;
	}

	bool operator != (const FileInfo &info)const
	{
		return this->_count != info._count;
	}

};

//文件压缩类
class FileCompress{
public:
	FileCompress()
	{
		for(int i = 0;i < 256;i++)
		{
			_infos[i]._ch = i;
		}
	}
	//文件压缩
	bool Compress(const char *inFileRoute, const char *outFileRoute, const char *fileName)
	{
		//打开文件
		assert(inFileRoute);
		assert(outFileRoute);
		assert(fileName);
		//处理文件名
		string inFile = inFileRoute;
		inFile += fileName;
		FILE *inputFile = fopen(inFile.c_str(), "rb");
		assert(inputFile);
		//统计字节总数
		char ch;
		LongType chSize = 0;//字节总数
		fread(&ch, 1, 1, inputFile);
		while (!feof(inputFile))
		{
			++chSize;
			_infos[(unsigned char)ch]._count++;
			fread(&ch, 1, 1, inputFile);
		}

		//哈夫曼树建立
		HuffmanTree<FileInfo,NodeCompare<FileInfo>> t;
		FileInfo invalid;              //非法值（字符没出现）
		short figColCount=t.Create(_infos, 256, invalid);
		_GenerateHuffmanCode(t.GetRoot());

		//配置文件
		string compressFileName = outFileRoute;
		compressFileName += fileName;
		compressFileName += ".huffman";
		FILE *fInCompress = fopen(compressFileName.c_str(), "wb");
		assert(fInCompress);
		
        //配置文件行数
		fwrite(&figColCount,2,1, fInCompress);
		//总字节数
		fwrite(&chSize, 8, 1, fInCompress);


		for (size_t i = 0; i < 256; ++i)
		{
			if (_infos[i]._count > 0)
			{
				//字符
				fwrite(&i, 1, 1, fInCompress);
				//字符计数
				fwrite(&_infos[i]._count, 8, 1, fInCompress);

			}
		}

	    //压缩文件
		fseek(inputFile, 0, SEEK_SET);
		fread(&ch, 1, 1, inputFile);
		int index = 0;
		unsigned char setBit = 0;
		while (!feof(inputFile))
		{
			string &code = _infos[(unsigned char)ch]._code;
			for(int i = 0;i < code.size();++i)
			{
				setBit <<= 1;
				if(code[i] == '1')
				{
					setBit |= 1;
				}
				if(++index == 8)
				{
					fwrite(&setBit, 1, 1, fInCompress);
					index = 0;
					setBit = 0;
				}
			}
			fread(&ch, 1, 1, inputFile);
		}
		if(index != 0)
		{
			setBit <<= (8-index);
			fwrite(&setBit, 1, 1, fInCompress);
		}
		fclose(inputFile);
		fclose(fInCompress);
		return true;
	}

	bool UnCompress(const char *inFileRoute, const char *outFileRoute, const char *fileName)
	{
		//判断文件是否可以解压
		string compressFileName = inFileRoute;
		compressFileName += fileName;
		if (compressFileName.find(".huffman") == compressFileName.npos)
		{
			cout << "非本程序压缩文件，不能解压" << endl;
			return false;
		}
		//打开待解压文件
		FILE *compressFile = fopen(compressFileName.c_str(), "rb");
		assert(compressFile);

		
		LongType chSize = 0;
		short figColCount = 0;
		//读取配置文件行数
		fread(&figColCount, 2, 1, compressFile);
		//读取总字节数
		fread(&chSize, 8, 1, compressFile);

		char ch;
		//读取配置文件
		while (figColCount)
		{
			fread(&ch, 1, 1, compressFile);
			fread(&_infos[(unsigned char)ch]._count, 8, 1, compressFile);
			--figColCount;
		}

		//构造huffman树
		HuffmanTree<FileInfo,NodeCompare<FileInfo>> t;
		FileInfo invalid;                
		t.Create(_infos,256,invalid);
		_GenerateHuffmanCode(t.GetRoot());

		//fseek(compressFile, 0, SEEK_SET);
		//写解压缩文件
		string outputFileName = outFileRoute;
		outputFileName += fileName;
		outputFileName.erase(outputFileName.find(".huffman"), 8);
		FILE* outputFile = fopen(outputFileName.c_str(), "r");
		//去除后缀并且判断输出是否重名
		if (outputFile)
		{
			outputFileName.insert(outputFileName.rfind('.'),1,'1');
		}
		outputFile = fopen(outputFileName.c_str(), "wb");
		assert(outputFile);

		//读取文件正文
		fread(&ch, 1, 1, compressFile);
		HuffmanTreeNode<FileInfo> *cur = t.GetRoot();
		int pos = 8;
		while (!feof(compressFile))
		{
			if (cur->_left == NULL && cur->_right == NULL)
			{
				//fputc(cur->_weight._ch, outputFile);
				fwrite(&cur->_weight._ch, 1, 1, outputFile);
				cur = t.GetRoot();
				if (--chSize == 0)
					break;
			}
			--pos;
			if (ch & (1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			if (pos == 0)
			{
				//ch = fgetc(compressFile);
				fread(&ch, 1, 1, compressFile);
				pos = 8;
			}
		} 

		fclose(outputFile);
		fclose(compressFile);
		return true;
	}
protected:
	void _GenerateHuffmanCode(HuffmanTreeNode<FileInfo>* root)
	{
		if(root == NULL)
			return;
		_GenerateHuffmanCode(root->_left);
		_GenerateHuffmanCode(root->_right);
		if(root->_left == NULL && root->_right == NULL)
		{
			HuffmanTreeNode<FileInfo>* cur = root;
			HuffmanTreeNode<FileInfo>* parent = cur->_parent;
			string & code = _infos[(unsigned char)cur->_weight._ch]._code;
			while(parent)
			{
				if(parent->_left == cur)
					code += '0';
				else
					code += '1';
				cur = parent;
				parent = cur->_parent;
			}
			reverse(code.begin(),code.end());
		}
	}
private:
	FileInfo _infos[256];
};