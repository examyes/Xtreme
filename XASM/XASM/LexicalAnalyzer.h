#pragma once

//词法分析器

#include <string>
#include <memory>
using std::string;

#include "utils.h"
#include "PublicDefine.h"

#include "SourceCodeHolder.h"

namespace XASM
{
	class CLexicalAnalyzer : public Singleton<CLexicalAnalyzer>
	{
		friend class Singleton<CLexicalAnalyzer>;
	private:
		enum ELexStatus
		{
			EN_LEX_NO_STRING = 0,		// 不在字符串中
			EN_LEX_IN_STRING = 1,		// 在字符串中
			EN_LEX_END_STRING = 2		// 字符串结束
		};
	private:
		CSourceCodeHolder m_source_holder;
		shared_ptr<CSourceCodeLine> m_curr_line;
		size_t m_index0;
		size_t m_index1;
		ELexStatus m_lex_status;
		string m_curr_lexeme;
		ETokenType m_curr_type;

	public:
		ETokenType get_next_token();
		string get_curr_lexeme();

		bool skip_to_next_line();
		//void reset();
		void set_source_holder(CSourceCodeHolder& holder);


	private:
		CLexicalAnalyzer(void){};
		~CLexicalAnalyzer(void){};
	};
}


