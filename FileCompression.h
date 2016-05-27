//��д��ݴ����ʱ����������һ�����飬��Զû����õģ���Զ��Ҫ���Լ�����
//���ͣ�coder
#pragma once 
#pragma warning(disable:4996)
#include"HuffmanTree.h"
#include<string.h>
#include<algorithm>

typedef  long long LongType;

//�����ļ��ṹ
struct FileInfo{
	char _ch;       //�ַ�
	LongType _count; //�ַ����ļ��г��ִ�����Ȩֵ��
	string _code;    //��Ӧ��������

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

//�ļ�ѹ����
class FileCompress{
public:
	FileCompress()
	{
		for(int i = 0;i < 256;i++)
		{
			_infos[i]._ch = i;
		}
	}
	//�ļ�ѹ��
	bool Compress(const char *inFileRoute, const char *outFileRoute, const char *fileName)
	{
		//���ļ�
		assert(inFileRoute);
		assert(outFileRoute);
		assert(fileName);
		//�����ļ���
		string inFile = inFileRoute;
		inFile += fileName;
		FILE *inputFile = fopen(inFile.c_str(), "rb");
		assert(inputFile);
		//ͳ���ֽ�����
		char ch;
		LongType chSize = 0;//�ֽ�����
		fread(&ch, 1, 1, inputFile);
		while (!feof(inputFile))
		{
			++chSize;
			_infos[(unsigned char)ch]._count++;
			fread(&ch, 1, 1, inputFile);
		}

		//������������
		HuffmanTree<FileInfo,NodeCompare<FileInfo>> t;
		FileInfo invalid;              //�Ƿ�ֵ���ַ�û���֣�
		short figColCount=t.Create(_infos, 256, invalid);
		_GenerateHuffmanCode(t.GetRoot());

		//�����ļ�
		string compressFileName = outFileRoute;
		compressFileName += fileName;
		compressFileName += ".huffman";
		FILE *fInCompress = fopen(compressFileName.c_str(), "wb");
		assert(fInCompress);
		
        //�����ļ�����
		fwrite(&figColCount,2,1, fInCompress);
		//���ֽ���
		fwrite(&chSize, 8, 1, fInCompress);


		for (size_t i = 0; i < 256; ++i)
		{
			if (_infos[i]._count > 0)
			{
				//�ַ�
				fwrite(&i, 1, 1, fInCompress);
				//�ַ�����
				fwrite(&_infos[i]._count, 8, 1, fInCompress);

			}
		}

	    //ѹ���ļ�
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
		//�ж��ļ��Ƿ���Խ�ѹ
		string compressFileName = inFileRoute;
		compressFileName += fileName;
		if (compressFileName.find(".huffman") == compressFileName.npos)
		{
			cout << "�Ǳ�����ѹ���ļ������ܽ�ѹ" << endl;
			return false;
		}
		//�򿪴���ѹ�ļ�
		FILE *compressFile = fopen(compressFileName.c_str(), "rb");
		assert(compressFile);

		
		LongType chSize = 0;
		short figColCount = 0;
		//��ȡ�����ļ�����
		fread(&figColCount, 2, 1, compressFile);
		//��ȡ���ֽ���
		fread(&chSize, 8, 1, compressFile);

		char ch;
		//��ȡ�����ļ�
		while (figColCount)
		{
			fread(&ch, 1, 1, compressFile);
			fread(&_infos[(unsigned char)ch]._count, 8, 1, compressFile);
			--figColCount;
		}

		//����huffman��
		HuffmanTree<FileInfo,NodeCompare<FileInfo>> t;
		FileInfo invalid;                
		t.Create(_infos,256,invalid);
		_GenerateHuffmanCode(t.GetRoot());

		//fseek(compressFile, 0, SEEK_SET);
		//д��ѹ���ļ�
		string outputFileName = outFileRoute;
		outputFileName += fileName;
		outputFileName.erase(outputFileName.find(".huffman"), 8);
		FILE* outputFile = fopen(outputFileName.c_str(), "r");
		//ȥ����׺�����ж�����Ƿ�����
		if (outputFile)
		{
			outputFileName.insert(outputFileName.rfind('.'),1,'1');
		}
		outputFile = fopen(outputFileName.c_str(), "wb");
		assert(outputFile);

		//��ȡ�ļ�����
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