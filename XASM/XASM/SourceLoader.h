#pragma once
#include <string>
#include "SourceCodeHolder.h"

//源代码文本加载器，将文本文件加载为预定格式（主要是换行符的处理）

class SourceLoader
{
public:
	SourceLoader(void);
	~SourceLoader(void);

public:
	static SourceCodeHolder loadSourceFile(std::string strFilePath);
};

