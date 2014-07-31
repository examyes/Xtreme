#include "StdAfx.h"
#include "PreProcessor.h"
#include <iostream>

PreProcessor::PreProcessor(void)
{

}


PreProcessor::~PreProcessor(void)
{

}

SourceCodeHolder PreProcessor::preProcess(SourceCodeHolder& preHolder)
{
	SourceCodeHolder holder;

	SourceCodeHolder::Iterator itor = preHolder.begin();
	while(itor != preHolder.end())
	{
		preProcessLine(holder, itor);
		++itor;
	}

	return holder;
}

void PreProcessor::preProcessLine(SourceCodeHolder& holder, SourceCodeHolder::Iterator itor)
{
	std::string strLine = itor->getSourceText();

	deleteNoteString(strLine);
	trimString(strLine);	
	if (!isNullLine(strLine))
	{	
		holder.appendSourceRow(SourceLine(strLine, itor->getRowIndex()));
	}
}

void PreProcessor::deleteNoteString(std::string& str)
{
	bool bInString = false;

	for(size_t index = 0; index < str.size(); ++index)
	{
		char ch = str.at(index);
		if (isStringNoteChar(ch))
		{
			bInString = !bInString;
		}
		else if (isSingleNoteChar(ch))
		{
			if (!bInString)
			{
				str = str.substr(0, index);
				return ;
			}
		}
	}
}

bool PreProcessor::isSingleNoteChar(char ch)
{
	return ch == ';';
}

bool PreProcessor::isStringNoteChar(char ch)
{
	return ch == '"';
}

bool PreProcessor::isNullLine(std::string strLine)
{
	return strLine.empty();
}

void PreProcessor::trimString(std::string& str)
{
	if (str.size() > 0)
	{
		int begin = getFirstNotWhiteCharPos(str);
		int end = getLastNotWhiteCharPos(str) + 1;

		if (-1 != begin)
		{
			str = str.substr(begin, end);
		}
		else
		{
			str = std::string("");
		}
	}
}

bool PreProcessor::isWhiteChar(char ch)
{
	return (ch == ' ') || (ch == '\t');
}

int PreProcessor::getFirstNotWhiteCharPos(std::string& str)
{
	for (int index = 0; index < str.size(); ++index)
	{
		if (!isWhiteChar(str.at(index)))
		{
			return index;
		}
	}

	return -1;
}

int PreProcessor::getLastNotWhiteCharPos(std::string& str)
{
	for (int index = str.size() - 1;index >= 0;--index)
	{
		if (!isWhiteChar(str.at(index)))
		{
			return index;
		}
	}

	return -1;	
}