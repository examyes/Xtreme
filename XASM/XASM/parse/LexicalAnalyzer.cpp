#include "LexicalAnalyzer.h"

#include "../utils/StringUtils.h"
#include "../data/InstrLookupTable.h"

namespace XASM
{
	void CLexicalAnalyzer::set_source_holder(CSourceCodeHolder& holder)
	{
		m_source_holder = holder;

		/// 区别处理第一行，如果在Loader时统一在最后加一个空行，好处理些
		if (m_source_holder.row_count() > 0)
		{
			m_curr_line = m_source_holder[0];
		}
		else
		{
			m_curr_line = shared_ptr<CSourceCodeLine>(NULL);
		}

		m_index0 = 0;
		m_index1 = 0;
		m_lex_status = EN_LEX_NO_STRING;
		m_curr_type = TOKEN_TYPE_INVALID;
	}

	void CLexicalAnalyzer::reset()
	{
		/// 区别处理第一行，如果在Loader时统一在最后加一个空行，好处理些
		if (m_source_holder.row_count() > 0)
		{
			m_curr_line = m_source_holder[0];
		}
		else
		{
			m_curr_line = shared_ptr<CSourceCodeLine>(NULL);
		}

		m_index0 = 0;
		m_index1 = 0;
		m_lex_status = EN_LEX_NO_STRING;
		m_curr_type = TOKEN_TYPE_INVALID;
	}

	ETokenType CLexicalAnalyzer::get_next_token()
	{
		if (!m_curr_line)
		{
			return TOKEN_TYPE_END_OF_STREAM;
		}

		m_index0 = m_index1;

		// 到达当前行末尾
		while (m_index0 >= m_curr_line->text().size())
		{
			if (!skip_to_next_line())
			{
				return TOKEN_TYPE_END_OF_STREAM;
			}
		}

		//
		if (EN_LEX_END_STRING == m_lex_status)
		{
			m_lex_status = EN_LEX_NO_STRING;
		}

		if (EN_LEX_IN_STRING != m_lex_status)
		{
			/// 不在字符串中，剔除空白先
			while (true)
			{
				if (!is_char_whitespace(m_curr_line->text().at(m_index0)))
				{
					break;
				}

				++m_index0;
			}
		}

		m_index1 = m_index0;
		while (true)
		{
			if (EN_LEX_IN_STRING == m_lex_status)
			{
				if (m_index1 >= m_curr_line->text().size())
				{
					return TOKEN_TYPE_INVALID;
				}

				if ('\\' == m_curr_line->text().at(m_index1))
				{
					m_index1 += 2;
					continue;
				}

				if ('"' == m_curr_line->text().at(m_index1))
				{
					break;
				}
				++m_index1;
			}
			else
			{
				if (m_index1 >= m_curr_line->text().size())
				{
					break;
				}

				if (is_char_delimiter(m_curr_line->text().at(m_index1)))
				{
					break;
				}
				++m_index1;
			}
		}

		if (0 == (m_index1 - m_index0))
		{
			++m_index1;
		}

		m_curr_lexeme.clear();
		for (size_t curr_index = m_index0;
			curr_index < m_index1;
			++curr_index)
		{
			if (EN_LEX_IN_STRING == m_lex_status)
			{
				if ('\\' == m_curr_line->text().at(curr_index))
				{
					continue;
				}
			}

			m_curr_lexeme += m_curr_line->text().at(curr_index);
		}

		m_curr_type = TOKEN_TYPE_INVALID;
		if (m_curr_lexeme.size() > 1 ||
			m_curr_lexeme.at(0) != '"')
		{
			if (m_lex_status == EN_LEX_IN_STRING)
			{
				m_curr_type = TOKEN_TYPE_STRING;
				return TOKEN_TYPE_STRING;
			}
		}

		if (1 == m_curr_lexeme.size())
		{
			switch (m_curr_lexeme.at(0))
			{
			case '"':
			{
						switch (m_lex_status)
						{
						case XASM::CLexicalAnalyzer::EN_LEX_NO_STRING:
							m_lex_status = EN_LEX_IN_STRING;
							break;
						case XASM::CLexicalAnalyzer::EN_LEX_IN_STRING:
							m_lex_status = EN_LEX_END_STRING;
							break;
						}

						m_curr_type = TOKEN_TYPE_QUOTE;
						break;
			}
			case ',':
			{
						m_curr_type = TOKEN_TYPE_COMMA;
						break;
			}
			case ':':
			{
						m_curr_type = TOKEN_TYPE_COLON;
						break;
			}
			case '[':
			{
						m_curr_type = TOKEN_TYPE_OPEN_BRACKET;
						break;
			}
			case ']':
			{
						m_curr_type = TOKEN_TYPE_CLOSE_BRACKET;
						break;
			}
			case '{':
			{
						m_curr_type = TOKEN_TYPE_OPEN_BRACE;
						break;
			}
			case '}':
			{
						m_curr_type = TOKEN_TYPE_CLOSE_BRACE;
						break;
			}
			case '\n':
			{
						 m_curr_type = TOKEN_TYPE_NEWLINE;
						 break;
			}
			}
		}

		if (is_string_float(m_curr_lexeme))
		{
			m_curr_type = TOKEN_TYPE_FLOAT;
		}

		if (is_string_int(m_curr_lexeme))
		{
			m_curr_type = TOKEN_TYPE_INT;
		}

		if (is_string_ident(m_curr_lexeme))
		{
			m_curr_type = TOKEN_TYPE_IDENTIFY;
		}

		if ("SETSTACKSIZE" == m_curr_lexeme)
		{
			m_curr_type = TOKEN_TYPE_SET_STACKSIZE;
		}

		if ("Var" == m_curr_lexeme)
		{
			m_curr_type = TOKEN_TYPE_VAR;
		}

		if ("Func" == m_curr_lexeme)
		{
			m_curr_type = TOKEN_TYPE_FUNC;
		}

		if ("Param" == m_curr_lexeme)
		{
			m_curr_type = TOKEN_TYPE_PARAM;
		}

		if ("_Retval" == m_curr_lexeme)
		{
			m_curr_type = TOKEN_TYPE_REG_RETVAL;
		}

		SInstrLookup instr;
		if (CInstrLookupTable::Instance()->get_instr_by_mnemonic(m_curr_lexeme))
		{
			m_curr_type = TOKEN_TYPE_INSTRUCTION;
		}

		return m_curr_type;
	}

	bool CLexicalAnalyzer::skip_to_next_line()
	{
		size_t curr_row = m_curr_line->row();
		if (curr_row >= m_source_holder.row_count())
		{
			return false;
		}

		m_curr_line = m_source_holder.at(curr_row);
		m_index0 = 0;
		m_index1 = 0;
		m_curr_lexeme.clear();
		m_lex_status = EN_LEX_NO_STRING;
		return true;
	}

	string CLexicalAnalyzer::get_curr_lexeme()
	{
		return m_curr_lexeme;
	}

	size_t CLexicalAnalyzer::get_curr_row()
	{
		return m_curr_line->row();
	}

	char CLexicalAnalyzer::peed_next()
	{
		shared_ptr<CSourceCodeLine> tmp_line = m_curr_line;
		int tmp_index = m_index1;

		if (EN_LEX_IN_STRING != m_lex_status)
		{
			while (true)
			{
				if (tmp_index >= tmp_line->text().size())
				{
					if (tmp_line->row() >= m_source_holder.row_count())
					{
						return 0;
					}

					tmp_line = m_source_holder.at(tmp_line->row());
					tmp_index = 0;

					continue;
				}

				if (!is_char_whitespace(tmp_line->text().at(tmp_index)))
				{
					break;
				}

				++tmp_index;
			}
		}

		return tmp_line->text().at(tmp_index);
	}
}
