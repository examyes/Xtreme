#include "SyntaxParser.h"

#include <map>
#include <functional>
using std::map;
using std::function;
using namespace std::placeholders;

#include "../utils/ErrorReporter.h"
#include "../data/SymbolTable.h"
#include "../data/FunctionTable.h"
#include "../data/LabelTable.h"
#include "../data/InstrLookupTable.h"

namespace XASM
{
	/// 这个语法分析很不健壮
	void CSyntaxParser::parse(CTokenStream& token_stream)
	{
		m_instr_stream_size = 0;
		m_is_stacksize_found = false;
		m_is_func_active = false;
		m_curr_func_local_data_size = 0;
		m_curr_func_index = 0;
		m_curr_func_name.clear();
		m_curr_func_param_count = 0;

		token_stream.reset();
		if (!parse_phase_1(token_stream))
		{
			return;
		}

		m_instr_stream_size = 0;
		m_is_stacksize_found = false;
		m_is_func_active = false;
		m_curr_func_local_data_size = 0;
		m_curr_func_index = 0;
		m_curr_func_name.clear();
		m_curr_func_param_count = 0;

		token_stream.reset();
		CInstrStream instr_stream;
		if (!parse_phase_2(token_stream, instr_stream))
		{
			return;
		}
	}

	bool CSyntaxParser::parse_phase_1(CTokenStream& token_stream)
	{
		/// 第一次遍历，主要收集数据
		static map<ETokenType, function<bool(CTokenStream&, shared_ptr<SToken>&)>> token_func_map = {
			{ TOKEN_TYPE_IDENTIFY, std::bind(&CSyntaxParser::phase_1_identify, this, _1, _2) },
			{ TOKEN_TYPE_CLOSE_BRACE, std::bind(&CSyntaxParser::phase_1_close_brace, this, _1, _2) },
			{ TOKEN_TYPE_INSTRUCTION, std::bind(&CSyntaxParser::phase_1_instruction, this, _1, _2) },
			{ TOKEN_TYPE_SET_STACKSIZE, std::bind(&CSyntaxParser::phase_1_stacksize, this, _1, _2) },
			{ TOKEN_TYPE_VAR, std::bind(&CSyntaxParser::phase_1_var, this, _1, _2) },
			{ TOKEN_TYPE_FUNC, std::bind(&CSyntaxParser::phase_1_func, this, _1, _2) },
			{ TOKEN_TYPE_PARAM, std::bind(&CSyntaxParser::phase_1_param, this, _1, _2) }
		};

		auto token_ptr = token_stream.next_token();
		while (token_ptr)
		{
			if (TOKEN_TYPE_END_OF_STREAM == token_ptr->type)
			{
				return true;
			}

			auto map_itor = token_func_map.find(token_ptr->type);
			if (token_func_map.end() != map_itor)
			{
				auto functor = map_itor->second;
				if (!functor(token_stream, token_ptr))
				{
					return false;
				}
			}
			else
			{
				if (!phase_1_default(token_stream, token_ptr))
				{
					return false;
				}
			}

			/// 此处需要的是跳转到下一行....但是不合理啊~~~
			/// 暂时按照跳转到下一行进行操作，在每行结尾都有一个TOKEN_TYPE_NEWLINE
			while (TOKEN_TYPE_NEWLINE != token_ptr->type)
			{
				token_ptr = token_stream.next_token();
			}

			token_ptr = token_stream.next_token();
		}

		return true;
	}

	/// 只解析LABEL
	bool CSyntaxParser::phase_1_identify(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		auto next_token = token_stream.next_token();
		if (TOKEN_TYPE_COLON != next_token->type)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_INSTR);
			return false;
		}

		if (!m_is_func_active)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_LINE_LABEL);
			return false;
		}

		if (-1 == CLabelTable::Instance()->add(token_ptr->lexeme,
			m_instr_stream_size - 1,
			m_curr_func_index))
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_LINE_LABEL_REDEFINITION);
			return false;
		}

		return true;
	}

	bool CSyntaxParser::phase_1_close_brace(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		if (!m_is_func_active)
		{
			CErrorReporter::Instance()->exit_on_char_expected_error('}');
			return false;
		}

		CFunctionTable::Instance()->set_func_info(m_curr_func_name,
			m_curr_func_param_count,
			m_curr_func_local_data_size);

		m_is_func_active = false;
		return true;
	}

	bool CSyntaxParser::phase_1_instruction(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		if (!m_is_func_active)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_INSTR);
			return false;
		}

		++m_instr_stream_size;
		return true;
	}

	bool CSyntaxParser::phase_1_stacksize(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		if (m_is_func_active)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_LOCAL_SETSTACKSIZE);
			return false;
		}

		if (m_is_stacksize_found)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_MULTIPLE_SETSTACKSIZES);
			return false;
		}

		auto next_token = token_stream.peek_next_token();
		if (TOKEN_TYPE_INT != next_token->type)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_STACK_SIZE);
			return false;
		}

		m_header.stack_size = atoi(token_ptr->lexeme.c_str());
		m_is_stacksize_found = true;
		return true;
	}

	bool CSyntaxParser::phase_1_var(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
			return false;
		}

		int size = 1;
		auto next_token = token_stream.peek_next_token();
		if (TOKEN_TYPE_OPEN_BRACKET == next_token->type)
		{
			next_token = token_stream.next_token();
			next_token = token_stream.next_token();

			if (TOKEN_TYPE_INT != next_token->type)
			{
				CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_ARRAY_SIZE);
				return false;
			}

			size = atoi(next_token->lexeme.c_str());
			if (size <= 0)
			{
				CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_ARRAY_SIZE);
				return false;
			}

			next_token = token_stream.next_token();
			if (TOKEN_TYPE_CLOSE_BRACKET != next_token->type)
			{
				CErrorReporter::Instance()->exit_on_char_expected_error(']');
				return false;
			}
		}

		int stack_index = m_is_func_active ? -(m_curr_func_local_data_size + 2) 
			: m_header.global_data_size;

		if (-1 == CSymbolTable::Instance()->add(token_ptr->lexeme,
			size,
			stack_index,
			m_curr_func_index))
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_REDEFINITION);
			return false;
		}

		if (m_is_func_active)
		{
			m_curr_func_local_data_size += size;
		}
		else
		{
			m_header.global_data_size += size;
		}
		return true;
	}

	bool CSyntaxParser::phase_1_func(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		if (m_is_func_active)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_NESTED_FUNC);
			return false;
		}

		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
			return false;
		}

		string str_name = token_ptr->lexeme;
		int entry_point = m_instr_stream_size;
		int func_index = CFunctionTable::Instance()->add(str_name, entry_point);
		if (-1 == func_index)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_FUNC_REDEFINITION);
			return false;
		}

		if (MAIN_FUNC_NAME == str_name)
		{
			m_header.is_mainfunc_present = true;
			m_header.mainfunc_index = func_index;
		}

		m_is_func_active = true;
		m_curr_func_name = str_name;
		m_curr_func_index = func_index;
		m_curr_func_param_count = 0;
		m_curr_func_local_data_size = 0;

		token_ptr = token_stream.next_token();
		while (token_ptr && TOKEN_TYPE_NEWLINE == token_ptr->type)
		{
			token_ptr = token_stream.next_token();
		};

		if (!token_ptr || TOKEN_TYPE_OPEN_BRACE != token_ptr->type)
		{
			CErrorReporter::Instance()->exit_on_char_expected_error('{');
			return false;
		}

		// 在FUNC的结果会自动添加RET指令，所以此处需要++
		++m_instr_stream_size;
		return true;
	}

	bool CSyntaxParser::phase_1_param(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		if (!m_is_func_active)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_PARAM);
		}

		if (m_curr_func_name == MAIN_FUNC_NAME)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_MAIN_PARAM);
		}

		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
			return false;
		}

		++m_curr_func_param_count;
		return true;
	}

	bool CSyntaxParser::phase_1_default(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr)
	{
		if (TOKEN_TYPE_NEWLINE != token_ptr->type)
		{
			CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_INPUT);
			return false;
		}

		return true;
	}


	bool CSyntaxParser::parse_phase_2(CTokenStream& token_stream, CInstrStream& instr_stream)
	{
		static map<ETokenType, function<bool(CTokenStream&, shared_ptr<SToken>&, CInstrStream&)>> token_func_map = {
			{ TOKEN_TYPE_CLOSE_BRACE, std::bind(&CSyntaxParser::phase_2_close_brace, this, _1, _2, _3) },
			{ TOKEN_TYPE_INSTRUCTION, std::bind(&CSyntaxParser::phase_2_instruction, this, _1, _2, _3) },
			{ TOKEN_TYPE_FUNC, std::bind(&CSyntaxParser::phase_2_func, this, _1, _2, _3) },
			{ TOKEN_TYPE_PARAM, std::bind(&CSyntaxParser::phase_2_param, this, _1, _2, _3) }
		};

		auto token_ptr = token_stream.next_token();
		while (token_ptr)
		{
			if (TOKEN_TYPE_END_OF_STREAM == token_ptr->type)
			{
				return true;
			}

			auto map_itor = token_func_map.find(token_ptr->type);
			if (token_func_map.end() != map_itor)
			{
				auto functor = map_itor->second;
				if (!functor(token_stream, token_ptr, instr_stream))
				{
					return false;
				}
			}

			/// 此处需要的是跳转到下一行....但是不合理啊~~~
			/// 暂时按照跳转到下一行进行操作，在每行结尾都有一个TOKEN_TYPE_NEWLINE
			while (TOKEN_TYPE_NEWLINE != token_ptr->type)
			{
				token_ptr = token_stream.next_token();
			}

			token_ptr = token_stream.next_token();
		}

		return true;
	}

	bool CSyntaxParser::phase_2_func(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream)
	{
		return true;
	};

	bool CSyntaxParser::phase_2_param(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream)
	{
		return true;
	};

	bool CSyntaxParser::phase_2_close_brace(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream)
	{
		return true;
	};

	bool CSyntaxParser::phase_2_instruction(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream)
	{
		return true;
	};
}