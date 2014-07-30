#include "StdAfx.h"
#include "SourceLoader.h"
#include "SourceLine.h"
#include <fstream>

SourceLoader::SourceLoader(void)
{

}


SourceLoader::~SourceLoader(void)
{

}

SourceCodeHolder SourceLoader::loadSourceFile(std::string strFilePath)
{
	SourceCodeHolder holder;

	unsigned int iRow = 1;
	std::fstream inFile(strFilePath.c_str());
	if (inFile)
	{
		std::string strLine;
		while (getline(inFile,strLine))
		{
			holder.appendSourceRow(SourceLine(strLine,iRow));
			++iRow;
		}
	}

	return holder;
}