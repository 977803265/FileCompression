#pragma once 
#pragma warning(disable:4996)
#include"HuffmanTree.h"
#include<string.h>
#include<algorithm>

typedef  long long LongType;

struct FileInfo{
	char _ch;
	LongType _count;
	string _code;

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

class FileCompress{
public:
	FileCompress()
	{
		for(int i = 0;i < 256;i++)
		{
			_infos[i]._ch = i;
		}
	}

	bool Compress(const char *filename)
	{
		assert(filename);
		FILE *fOut = fopen(filename,"rb");
		assert(fOut);
		char ch = fgetc(fOut);
		LongType chSize = 0;
		while (!feof(fOut))
		{
			++chSize;
			_infos[(unsigned char)ch]._count++;
			ch = fgetc(fOut);
		}
		HuffmanTree<FileInfo,NodeCompare<FileInfo>> t;
		FileInfo invalid;                //非法值
		t.Create(_infos,256,invalid);
		_GenerateHuffmanCode(t.GetRoot());

	    //压缩文件
		string compressfile = filename;
		compressfile += ".huffman";
		FILE *fInCompress = fopen(compressfile.c_str(),"wb");
		assert(fInCompress);

		fseek(fOut,0,SEEK_SET);
		ch = fgetc(fOut);
		int index = 0;
		unsigned char setBit = 0;
		while (!feof(fOut))
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
					fputc(setBit,fInCompress);
					index = 0;
					setBit = 0;
				}
			}
			ch = fgetc(fOut);
		}
		if(index != 0)
		{
			setBit <<= (8-index);
			fputc(setBit,fInCompress);
		}
		//配置文件
		string configfile = filename;
		configfile += ".cfig";
		FILE *fInConfig = fopen(configfile.c_str(),"wb");
		assert(fInConfig);

		char str[32];
		itoa((int)(chSize >> 32), str, 10);
		fputs(str, fInConfig);
		fputc('\n', fInConfig);
		itoa((int)chSize, str, 10);
		fputs(str, fInConfig);
		fputc(' \n', fInConfig);

		for(size_t i = 0;i < 256;++i)
		{
			string Inconfig;
			if(_infos[i]._count > 0)
			{
				Inconfig += _infos[i]._ch;
				Inconfig += ',';
				Inconfig += itoa((int)(_infos[i]._count >> 32), str, 10);
				Inconfig += '\n';
				fputs(Inconfig.c_str(), fInConfig);
				Inconfig.clear();
				Inconfig += itoa((int)_infos[i]._count, str, 10);
				Inconfig += '\n';
				fputs(Inconfig.c_str(), fInConfig);
			}
		//	fputs(Inconfig.c_str(),fInConfig);
		}
		fclose(fOut);
		fclose(fInCompress);
		fclose(fInConfig);
		return true;
	}

	bool UnCompress(const char *filename)
	{
		//读取配置文件
		
		string configfile = filename;
		configfile += ".cfig";
		FILE *fOutConfig = fopen(configfile.c_str(),"rb");
		assert(fOutConfig);

		string line;
		LongType chSize = 0;
		ReadLine(fOutConfig,line);
		chSize = (LongType)atoi(line.c_str());
		chSize <<= 32;
		line.clear();
		ReadLine(fOutConfig, line);
		chSize += (LongType)atoi(line.c_str());
	  //  chSize << 32;
	//	line.clear();
	//	ReadLine(fOutConfig,line);
	//	chSize += atoi(line.c_str());
		line.clear();

		while(ReadLine(fOutConfig,line))
		{
			unsigned char ch = 0;
			if(!line.empty())
			{
				ch = line[0];
				_infos[ch]._count = (LongType)atoi(line.substr(2).c_str());
				_infos[ch]._count <<= 32;
				line.clear();
			}
			else
			{
				line = '\n';
				ReadLine(fOutConfig, line);
				ch = line[0];
				_infos[ch]._count = (LongType)atoi(line.substr(2).c_str());
				_infos[ch]._count <<= 32;
				line.clear();
			}
			ReadLine(fOutConfig, line);
			_infos[ch]._count +=(int) atoi(line.substr().c_str());
			line.clear();
		}

		//构造huffman树
		HuffmanTree<FileInfo,NodeCompare<FileInfo>> t;
		FileInfo invalid;                //_count是0
		t.Create(_infos,256,invalid);
		_GenerateHuffmanCode(t.GetRoot());

		//解压缩
		string compressfile = filename;   //读压缩文件
		compressfile += ".huffman";
		FILE *fOutCompress = fopen(compressfile.c_str(),"rb");
		assert(fOutCompress);

		string uncompressfile = filename;    //写解压缩文件
		uncompressfile += ".uncompress";
		FILE *fInUncompress = fopen(uncompressfile.c_str(),"wb");
		assert(fInUncompress);

		char ch = fgetc(fOutCompress);
		HuffmanTreeNode<FileInfo> *cur = t.GetRoot();
		int pos = 8;
		while (!feof(fInUncompress))
		{
			if(cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,fInUncompress);
				cur = t.GetRoot();
				if(--chSize == 0)
					break;
			}
			--pos;
			if(ch & (1<<pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			if(pos == 0)
			{
				ch = fgetc(fOutCompress);
				pos = 8;
			}
		}
		fclose(fOutConfig);
		fclose(fOutCompress);
		fclose(fInUncompress);
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
	
	bool ReadLine(FILE *fOut,string &str)
	{
		char ch =fgetc(fOut);
		if(ch == EOF)
			return false;
		while(ch != EOF && ch != '\n')
		{
			str += ch;
			ch = fgetc(fOut);
		}
		return true;
	}
private:
	FileInfo _infos[256];
};