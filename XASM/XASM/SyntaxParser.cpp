#include "SyntaxParser.h"

#include "LexicalAnalyzer.h"
#include "ErrorReporter.h"
#include "SymbolTable.h"
#include "FunctionTable.h"
#include "LabelTable.h"

namespace XASM
{
	void CSyntaxParser::parse()
	{
		int instr_stream_size = 0;
		bool is_stacksize_found = false;
		bool is_func_active = false;

		int curr_func_local_data_size = 0;
		int curr_func_index = 0;
		string curr_func_name;
		int curr_func_param_count = 0;
		
		CLexicalAnalyzer* p_lex = CLexicalAnalyzer::Instance();

		p_lex->reset();
		ETokenType token;

		while (true)
		{
			token = p_lex->get_next_token();
			if (TOKEN_TYPE_END_OF_STREAM == token)
			{
				break;
			}

			switch (token)
			{
			case XASM::TOKEN_TYPE_IDENTIFY:
			{
				if (':' != p_lex->peed_next())
				{
					CErrorReporter::Instance()->exit_on_error("invalid instr");
				}

				if (!is_func_active)
				{
					CErrorReporter::Instance()->exit_on_error("global label");
				}

				string str_ident = p_lex->get_curr_lexeme();
				int target_index = instr_stream_size - 1;
				int func_index = curr_func_index;

				if (-1 == CLabelTable::Instance()->add(str_ident, 
					target_index, 
					func_index))
				{
					CErrorReporter::Instance()->exit_on_error("label redefine");
				}
			}
				break;
			case XASM::TOKEN_TYPE_CLOSE_BRACE:
			{
				if (!is_func_active)
				{
					CErrorReporter::Instance()->exit_on_char_expected_error('}');
				}

				CFunctionTable::Instance()->set_func_info(curr_func_name, 
					curr_func_param_count, 
					curr_func_local_data_size);

				is_func_active = false;
			}
				break;
			case XASM::TOKEN_TYPE_INSTRUCTION:
			{
				if (!is_func_active)
				{
					CErrorReporter::Instance()->exit_on_error("global instr");
				}

				++instr_stream_size;
			}
				break;
			case XASM::TOKEN_TYPE_SET_STACKSIZE:
			{
				if (is_func_active)
				{
					CErrorReporter::Instance()->exit_on_error("stack");
				}

				if (is_stacksize_found)
				{
					CErrorReporter::Instance()->exit_on_error("found");
				}

				ETokenType next_token = p_lex->get_next_token();
				if (TOKEN_TYPE_INT != next_token)
				{
					CErrorReporter::Instance()->exit_on_error("int size");
				}

				m_header.stack_size = atoi(p_lex->get_curr_lexeme().c_str());
				is_stacksize_found = true;
			}
				break;
			case XASM::TOKEN_TYPE_VAR:
			{
				if (token != TOKEN_TYPE_IDENTIFY)
				{
					CErrorReporter::Instance()->exit_on_error("indent");
				}

				string str_ident = p_lex->get_curr_lexeme();
				int size = 1;

				if ('[' == p_lex->peed_next())
				{
					ETokenType next_token = p_lex->get_next_token();
					if (next_token != TOKEN_TYPE_OPEN_BRACKET)
					{
						CErrorReporter::Instance()->exit_on_char_expected_error('[');
					}

					next_token = p_lex->get_next_token();
					if (TOKEN_TYPE_INT != next_token)
					{
						CErrorReporter::Instance()->exit_on_error("int");
					}

					size = atoi(p_lex->get_curr_lexeme().c_str());
					if (size <= 0)
					{
						CErrorReporter::Instance()->exit_on_error("invalid");
					}

					next_token = p_lex->get_next_token();
					if (TOKEN_TYPE_CLOSE_BRACKET != next_token)
					{
						CErrorReporter::Instance()->exit_on_char_expected_error(']');
					}
				}

				int stack_index = 0;
				if (is_func_active)
				{
					stack_index = -(curr_func_local_data_size + 2);
				}
				else
				{
					stack_index = m_header.global_data_size;
				}

				if (CSymbolTable::Instance()->add(str_ident, 
					size, 
					stack_index, 
					curr_func_index) == -1)
				{
					CErrorReporter::Instance()->exit_on_error("symbol add error");
				}

				if (is_func_active)
				{
					curr_func_local_data_size += size;
				}
				else
				{
					m_header.global_data_size += size;
				}
			}
				break;
			case XASM::TOKEN_TYPE_FUNC:
			{
				if (is_func_active)
				{
					CErrorReporter::Instance()->exit_on_error("nest");
				}

				ETokenType next_token = p_lex->get_next_token();
				if (TOKEN_TYPE_IDENTIFY != next_token)
				{
					CErrorReporter::Instance()->exit_on_error("ident");
				}

				string str_name = p_lex->get_curr_lexeme();
				int entry_point = instr_stream_size;
				int func_index = CFunctionTable::Instance()->add(str_name, entry_point);
				if (-1 == func_index)
				{
					CErrorReporter::Instance()->exit_on_error("redefine");
				}

				if ("_Main" == str_name)
				{
					m_header.is_mainfunc_present = true;
					m_header.mainfunc_index = func_index;
				}

				is_func_active = true;
				curr_func_name = str_name;
				curr_func_index = func_index;
				curr_func_param_count = 0;
				curr_func_local_data_size = 0;

				next_token = p_lex->get_next_token();
				while (TOKEN_TYPE_NEWLINE == next_token)
				{
					
				};

				if (next_token != TOKEN_TYPE_OPEN_BRACE)
				{
					CErrorReporter::Instance()->exit_on_char_expected_error('{');
				}

				++instr_stream_size;
				
			}
				break;
			case XASM::TOKEN_TYPE_PARAM:
			{
				if (!is_func_active)
				{
					CErrorReporter::Instance()->exit_on_error("global param");
				}

				if (curr_func_name == "_Main")
				{
					CErrorReporter::Instance()->exit_on_error("main param");
				}

				ETokenType next_token = p_lex->get_next_token();
				if (next_token != TOKEN_TYPE_IDENTIFY)
				{
					CErrorReporter::Instance()->exit_on_error("indent");
				}

				++curr_func_param_count;
			}
				break;
			default:
			{
				if (token != TOKEN_TYPE_NEWLINE)
				{
					CErrorReporter::Instance()->exit_on_error("invalid input");
				}
			}
				break;
			}
		
			if (!p_lex->skip_to_next_line())
			{
				break;
			}
		}


	}
}