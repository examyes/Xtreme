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
		std::string strLine = itor->getSourceText();

		trimString(strLine);
		if (!isNullLine(strLine))
		{
			holder.appendSourceRow(SourceLine(strLine, itor->getRowIndex()));
		}
		++itor;
	}

	return holder;
}

std::string PreProcessor::getSingleNotes()
{
	return std::string(";");
}

bool PreProcessor::isNullLine(std::string strLine)
{
	return strLine.empty();
}

void PreProcessor::trimString(std::string& str)
{
	if (str.size() > 0)
	{
		str = str.substr(getFirstNotWhiteCharPos(str), getLastNotWhiteCharPos(str) + 1);
	}
}

bool PreProcessor::isWhiteChar(char ch)
{
	return (ch == ' ') || (ch == '\t');
}

int PreProcessor::getFirstNotWhiteCharPos(std::string& str)
{
	for (int index = 0;index < str.size();++index)
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