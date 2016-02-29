#pragma once
#include <string>

#include "SourceCodeHolder.h"

namespace XASM
{
	class CSourceLoader
	{
	private:
		CSourceLoader(void) {};
		~CSourceLoader(void) {};

	public:
		static CSourceCodeHolder load_file(std::string& file_path);
	};
}


