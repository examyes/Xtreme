#pragma once
#include <string>

#include "SourceCodeHolder.h"

namespace XASM
{
	class CSourceLoader
	{
	public:
		CSourceLoader(void);
		~CSourceLoader(void);

	public:
		static SourceCodeHolder load_file(std::string& strFilePath);
	};
}


