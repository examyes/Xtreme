#include "SourceLoader.h"

#include <fstream>
#include "SourceLine.h"

namespace XASM
{
	CSourceLoader::CSourceLoader(void)
	{

	}


	CSourceLoader::~CSourceLoader(void)
	{

	}

	SourceCodeHolder CSourceLoader::load_file(std::string& strFilePath)
	{
		SourceCodeHolder holder;

		unsigned int iRow = 1;
		std::fstream inFile(strFilePath.c_str());
		if (inFile)
		{
			std::string strLine;
			while (getline(inFile, strLine))
			{
				holder.appendSourceRow(CSourceLine(strLine, iRow));
				++iRow;
			}
		}

		return holder;
	}
}
