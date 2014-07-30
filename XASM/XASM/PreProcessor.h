#pragma once
#include <string>
#include "SourceCodeHolder.h"


//预处理器，主要作用是删除注释与去掉空行

class PreProcessor
{
public:
	PreProcessor(void);
	~PreProcessor(void);

public:
	static SourceCodeHolder preProcess(SourceCodeHolder& preHolder);

private:
	static std::string getSingleNotes();

	static bool isNullLine(std::string strLine);
	static void trimString(std::string& str);
	static bool isWhiteChar(char ch);

	static int getFirstNotWhiteCharPos(std::string& str);
	static int getLastNotWhiteCharPos(std::string& str);
};

