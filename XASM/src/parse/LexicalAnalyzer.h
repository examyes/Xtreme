#pragma once

// 词法分析器

#include <string>
#include <memory>
using std::string;

#include <liter/utils/Singleton.h>
#include "../data/PublicDefine.h"

#include "../data/SourceCodeHolder.h"
#include "../data/TokenStream.h"

namespace XASM
{

class CLexicalAnalyzer : public liter::Singleton<CLexicalAnalyzer>
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
		ELexStatus m_lex_status;
		string m_file_name;
		string m_lexeme;
		bool m_escape_find;

private:

    class WalkHelper
		{
		private:
        CSourceCodeHolder m_src;
        CSourceCodeHolder::iterator m_curr;
        size_t m_curr_index;

    public:
        static const char END_CHAR = -1;

    public:
        WalkHelper(CSourceCodeHolder& holder);

    public:
        char next();
        size_t row();

    private:
        bool skip_to_next_line();
		};

public:
		CTokenStream analyze(CSourceCodeHolder& src);

private:
		CLexicalAnalyzer(void){};
		~CLexicalAnalyzer(void){};

		void analyze_char(char val_ch, size_t row, CTokenStream& token_stream);
		void analyze_char_in_string(char val_ch, size_t row, CTokenStream& token_stream);
		void analyze_char_out_string(char val_ch, size_t row, CTokenStream& token_stream);

		void add_token(size_t row, ETokenType type, CTokenStream& token_stream);
		void parse_lexeme_to_token(size_t row, CTokenStream& token_stream);
		ETokenType parse_token_type_from_lexeme();
};

}


