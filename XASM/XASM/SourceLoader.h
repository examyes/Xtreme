#pragma once
#include <string>
#include "SourceCodeHolder.h"

//Դ�����ı������������ı��ļ�����ΪԤ����ʽ����Ҫ�ǻ��з��Ĵ���

class SourceLoader
{
public:
	SourceLoader(void);
	~SourceLoader(void);

public:
	static SourceCodeHolder loadSourceFile(std::string strFilePath);
};

