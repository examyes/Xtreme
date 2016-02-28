#pragma once

//´Ê·¨·ÖÎöÆ÷

#include <string>
using std::string;

#include "utils.h"
#include "PublicDefine.h"

namespace XASM
{
	class CLexicalAnalyzer : public Singleton<CLexicalAnalyzer>
	{
		friend class Singleton<CLexicalAnalyzer>;

	public:
		//ETokenType get_next_token();
		//string get_curr_lexeme();

		//void skip_to_next_line();
		//void reset();

	private:
		CLexicalAnalyzer(void){};
		~CLexicalAnalyzer(void){};
	};
}


