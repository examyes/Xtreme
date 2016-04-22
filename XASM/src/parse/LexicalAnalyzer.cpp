#include "LexicalAnalyzer.h"

#include <functional>
#include <map>
using std::function;
using std::map;

#include "../utils/StringUtils.h"
#include "../data/InstrLookupTable.h"

namespace XASM
{
	CLexicalAnalyzer::WalkHelper::WalkHelper(CSourceCodeHolder& holder)
		: m_src(holder)
		, m_curr_index(0)
	{
		m_curr = m_src.begin();
	}

	char CLexicalAnalyzer::WalkHelper::next()
	{
		if (!skip_to_next_line())
		{
			return END_CHAR;
		}

		auto val_ch = (*m_curr)->text().at(m_curr_index);
		m_curr_index++;
		return val_ch;
	}

	size_t CLexicalAnalyzer::WalkHelper::row()
	{
		return (*m_curr)->row();
	}

	bool CLexicalAnalyzer::WalkHelper::skip_to_next_line()
	{
		if (m_curr == m_src.end())	// 已经到代码末尾
		{
			return false;
		}

		if (m_curr_index >= (*m_curr)->text().size())
		{
			++m_curr;
			m_curr_index = 0;
			return skip_to_next_line();
		}

		return true;
	}

	void CLexicalAnalyzer::analyze_char(char val_ch, size_t row, CTokenStream& token_stream)
	{
		if (EN_LEX_END_STRING == m_lex_status)
		{
			m_lex_status = EN_LEX_NO_STRING;
		}

		if (EN_LEX_IN_STRING != m_lex_status && is_char_whitespace(val_ch))
		{
			// 空白符不在字符串中，尝试解析一次
			parse_lexeme_to_token(row, token_stream);

			// 
			return;
		}

		if (EN_LEX_IN_STRING == m_lex_status)
		{
			analyze_char_in_string(val_ch, row, token_stream);
		}
		else
		{
			analyze_char_out_string(val_ch, row, token_stream);
		}
	}

	void CLexicalAnalyzer::analyze_char_in_string(char val_ch, size_t row, CTokenStream& token_stream)
	{
		if ('\n' == val_ch)
		{
			m_lexeme += '\n';
			parse_lexeme_to_token(row, token_stream);
			m_lex_status = EN_LEX_END_STRING;
		}
		else if ('\\' == val_ch)
		{
			m_escape_find = true;
			return;
		}
		else if ('"' == val_ch && false == m_escape_find)
		{
			//parse_lexeme_to_token(row, token_stream); // 此处不能调用它，会被解析为invalid
			add_token(row, TOKEN_TYPE_STRING, token_stream);

			m_lexeme = '"';
			parse_lexeme_to_token(row, token_stream);
			m_lex_status = EN_LEX_END_STRING;
		}
		else
		{
			m_lexeme += val_ch;
			m_escape_find = false;
		}
	}

	void CLexicalAnalyzer::analyze_char_out_string(char val_ch, size_t row, CTokenStream& token_stream)
	{
		if ('"' == val_ch)
		{
			parse_lexeme_to_token(row, token_stream);
			m_lexeme = '"';
			parse_lexeme_to_token(row, token_stream);
			m_lex_status = EN_LEX_IN_STRING;
			m_escape_find = false;
			return;
		}

		if ('\n' == val_ch)
		{
			parse_lexeme_to_token(row, token_stream);
			m_lexeme = '\n';
			parse_lexeme_to_token(row, token_stream);
			return;
		}

		if (is_char_delimiter(val_ch))
		{
			parse_lexeme_to_token(row, token_stream);
			m_lexeme = val_ch;
			parse_lexeme_to_token(row, token_stream);
			return;
		}

		m_lexeme += val_ch;
	}

	void CLexicalAnalyzer::parse_lexeme_to_token(size_t row, CTokenStream& token_stream)
	{
		if (m_lexeme.empty())
		{
			return;
		}

		add_token(row, parse_token_type_from_lexeme(), token_stream);
		m_lexeme.clear();
	}

	ETokenType CLexicalAnalyzer::parse_token_type_from_lexeme()
	{
		static map<string, ETokenType> string_2_type = {
			{ "\"", TOKEN_TYPE_QUOTE },
			{ ",", TOKEN_TYPE_COMMA },
			{ ":", TOKEN_TYPE_COLON },
			{ "[", TOKEN_TYPE_OPEN_BRACKET },
			{ "]", TOKEN_TYPE_CLOSE_BRACKET },
			{ "{", TOKEN_TYPE_OPEN_BRACE },
			{ "}", TOKEN_TYPE_CLOSE_BRACE },
			{ "\n", TOKEN_TYPE_NEWLINE },
			{ "SETSTACKSIZE", TOKEN_TYPE_SET_STACKSIZE },
			{ "Var", TOKEN_TYPE_VAR },
			{ "Func", TOKEN_TYPE_FUNC },
			{ "Param", TOKEN_TYPE_PARAM },
			{ "_Retval", TOKEN_TYPE_REG_RETVAL }
		};

		auto string_2_itor = string_2_type.find(m_lexeme);
		if (string_2_type.end() != string_2_itor)
		{
			return string_2_itor->second;
		}

		if (is_string_float(m_lexeme))
		{
			return TOKEN_TYPE_FLOAT;
		}

		if (is_string_int(m_lexeme))
		{
			return TOKEN_TYPE_INT;
		}

		SInstrLookup instr;
		if (CInstrLookupTable::Instance()->get_instr_by_mnemonic(m_lexeme))
		{
			return TOKEN_TYPE_INSTRUCTION;
		}

		if (is_string_ident(m_lexeme))
		{
			return TOKEN_TYPE_IDENTIFY;
		}

		return TOKEN_TYPE_INVALID;
	}

	void CLexicalAnalyzer::add_token(size_t row, ETokenType type, CTokenStream& token_stream)
	{
		SToken val_token;
		val_token.type = type;
		val_token.row = row;
		val_token.file_name = m_file_name;
		val_token.lexeme = m_lexeme;

		token_stream.push_back(val_token);
	}

	CTokenStream CLexicalAnalyzer::analyze(CSourceCodeHolder& src)
	{
		CTokenStream token_stream;
		m_lex_status = EN_LEX_NO_STRING;
		m_file_name = src.file_name();
		m_lexeme.clear();
		m_escape_find = false;

		if (src.row_count() > 0)
		{
			WalkHelper w_helper(src);
			auto val_ch = w_helper.next();
			while (WalkHelper::END_CHAR != val_ch)
			{
				analyze_char(val_ch, w_helper.row(), token_stream);
				val_ch = w_helper.next();
			}
		}

		/// 添加一个结束TOKEN
		SToken end_token;
		end_token.file_name = src.file_name();
		end_token.lexeme = "";
		end_token.row = 0;
		end_token.type = TOKEN_TYPE_END_OF_STREAM;
		token_stream.push_back(end_token);

		return token_stream;
	}
}
