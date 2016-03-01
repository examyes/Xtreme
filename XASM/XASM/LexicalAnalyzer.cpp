#include "LexicalAnalyzer.h"

#include "StringUtils.h"

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

		/// 到此处，已解析成一个个的标识符
		return TOKEN_TYPE_COMMA;
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
}
