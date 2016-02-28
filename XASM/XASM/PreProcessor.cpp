#include "PreProcessor.h"
#include <iostream>

#include "StringUtils.h"

CPreProcessor::CPreProcessor(void)
{

}


CPreProcessor::~CPreProcessor(void)
{

}

SourceCodeHolder CPreProcessor::pre_process(SourceCodeHolder& preHolder)
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

void CPreProcessor::preProcessLine(SourceCodeHolder& holder, SourceCodeHolder::Iterator itor)
{
	std::string strLine = itor->getSourceText();

	deleteNoteString(strLine);
	trimString(strLine);	
	if (!isNullLine(strLine))
	{	
		holder.appendSourceRow(CSourceLine(strLine, itor->getRowIndex()));
	}
}

void CPreProcessor::deleteNoteString(std::string& str)
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

bool CPreProcessor::isSingleNoteChar(char ch)
{
	return ch == ';';
}

bool CPreProcessor::isStringNoteChar(char ch)
{
	return ch == '"';
}

bool CPreProcessor::isNullLine(std::string strLine)
{
	return strLine.empty();
}

void CPreProcessor::trimString(std::string& str)
{
	str = XASM::trim(str);
}

bool CPreProcessor::isWhiteChar(char ch)
{
	return (ch == ' ') || (ch == '\t');
}

int CPreProcessor::getFirstNotWhiteCharPos(std::string& str)
{
	for (std::string::size_type index = 0; index < str.size(); ++index)
	{
		if (!isWhiteChar(str.at(index)))
		{
			return index;
		}
	}

	return -1;
}

int CPreProcessor::getLastNotWhiteCharPos(std::string& str)
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