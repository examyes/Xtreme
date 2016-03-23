#include "ErrorReporter.h"

#include <iostream>

namespace XASM
{
	void CErrorReporter::exit_on_error(const char* msg)
	{
		std::cout << "Fatal Error: " << msg << std::endl;
		exit(0);
	}

	void CErrorReporter::exit_on_code_error(const char* msg, int row, const char* src)
	{
		std::cout << "Error: " << msg << std::endl;
		std::cout << "Line: " << row << std::endl;
		std::cout << src << std::endl;
		std::cout << "Could not assemble." << std::endl;
		exit(0);
	}

	void CErrorReporter::exit_on_char_expected_error(char ch)
	{
		std::cout << ch << " expected" << std::endl;
		exit(0);
	}
}