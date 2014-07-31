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
	static bool isSingleNoteChar(char ch);
	static bool isStringNoteChar(char ch);

	static bool isNullLine(std::string strLine);
	static void trimString(std::string& str);
	static bool isWhiteChar(char ch);

	static int getFirstNotWhiteCharPos(std::string& str);
	static int getLastNotWhiteCharPos(std::string& str);

	static void preProcessLine(SourceCodeHolder& holder, SourceCodeHolder::Iterator itor);
	static void deleteNoteString(std::string& str);
};

