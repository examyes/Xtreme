#include "SourceLoader.h"

#include <fstream>

namespace XASM
{
	CSourceCodeHolder CSourceLoader::load_file(std::string& file_path)
	{
		CSourceCodeHolder holder;

		size_t iRow = 1;
		std::fstream inFile(file_path.c_str());
		if (inFile)
		{
			std::string strLine;
			while (getline(inFile, strLine))
			{
				holder.append(strLine, iRow);
				++iRow;
			}
		}

		return holder;
	}
}
