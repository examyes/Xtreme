#pragma once

#include "utils.h"

namespace XASM
{
	class CErrorReporter : public Singleton<CErrorReporter>
	{
		friend class Singleton<CErrorReporter>;
	public:
		void exit_on_error(const char* msg);
		void exit_on_code_error(const char* msg, int row, const char* src);
		void exit_on_char_expected_error(char ch);

	private:
		CErrorReporter() {};
		~CErrorReporter() {};
	};

}


