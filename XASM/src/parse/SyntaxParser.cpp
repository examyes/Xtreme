#include "SyntaxParser.h"

#include <map>
#include <functional>
using std::map;
using std::function;
using std::bind;
using namespace std::placeholders;

#include "../utils/ErrorReporter.h"
#include "../data/SymbolTable.h"
#include "../data/FunctionTable.h"
#include "../data/LabelTable.h"
#include "../data/InstrLookupTable.h"

namespace XASM
{

namespace detail
{

// ���ݵ�ǰ���Է�������Ӧ�Ľ�������
SyntaxParserPhase1::ParseFuncType SyntaxParserPhase1::get_parse_func(ETokenType token_type) const
{
    using TokenFuncMap = map<ETokenType, ParseFuncType>;
		static TokenFuncMap token_func_map = {
        {
            TOKEN_TYPE_IDENTIFY,
            bind(&SyntaxParserPhase1::parse_identify, this, _1, _2)
        },
        {
            TOKEN_TYPE_CLOSE_BRACE,
            bind(&SyntaxParserPhase1::parse_close_brace, this, _1, _2)
        },
        {
            TOKEN_TYPE_INSTRUCTION,
            bind(&SyntaxParserPhase1::parse_instruction, this, _1, _2)
        },
        {
            TOKEN_TYPE_SET_STACKSIZE,
            bind(&SyntaxParserPhase1::parse_stacksize, this, _1, _2)
        },
        {
            TOKEN_TYPE_VAR,
            bind(&SyntaxParserPhase1::parse_var, this, _1, _2)
        },
        {
            TOKEN_TYPE_FUNC,
            bind(&SyntaxParserPhase1::parse_func, this, _1, _2)
        },
        {
            TOKEN_TYPE_PARAM,
            bind(&SyntaxParserPhase1::parse_param, this, _1, _2)
        }
		};

    auto map_itor = token_func_map.find(token_type);
    if (token_func_map.end() != map_itor)
    {
        return map_itor->second;
    }
    else
    {
        return bind(&SyntaxParserPhase1::parse_default, this, _1, _2);
    }

}

// ���������������������Է���
bool SyntaxParserPhase1::parse( XASM::CTokenStream &token_stream )
{
    SyntaxVarible::Instance()->reset();
    token_stream.reset();

    auto token_ptr = token_stream.next_token();
		while (token_ptr)
		{
        // �������Է���ĩβ
        if (TOKEN_TYPE_END_OF_STREAM == token_ptr->type)
        {
            return true;
        }

        // ��Բ�ͬ�����Է����ò�ͬ�Ĵ�����
        auto functor = get_parse_func(token_ptr->type);
        if (!functor(token_stream, token_ptr))
        {
            return false;
        }

        /// �˴���Ҫ������ת����һ��....���ǲ�����~~~
        /// ��ʱ������ת����һ�н��в�������ÿ�н�β����һ��TOKEN_TYPE_NEWLINE
        while (TOKEN_TYPE_NEWLINE != token_ptr->type)
        {
            token_ptr = token_stream.next_token();
        }

        token_ptr = token_stream.next_token();
		}

    return true;
}

// ������ʶ������, �˴��ı�ʶ������Ϊ��ŵģ���Ϊ�����ı�ʶ��Ӧ��������ָ���д���
bool SyntaxParserPhase1::parse_identify( XASM::CTokenStream &token_stream,
                                         shared_ptr<XASM::SToken> &token_ptr )
{
		auto next_token = token_stream.next_token();

    // ��ʶ����������ð��
		if (TOKEN_TYPE_COLON != next_token->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_INSTR);
        return false;
		}

    // ��ű����ں�����
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_LINE_LABEL);
        return false;
		}

    // ��ӵ���ű���
		if (-1 == CLabelTable::Instance()->add(token_ptr->lexeme,
                                           SyntaxVarible::Instance()->m_instr_stream_size - 1,
                                           SyntaxVarible::Instance()->m_curr_func_index))
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_LINE_LABEL_REDEFINITION);
        return false;
		}

		return true;
}

// �����Ҵ�����
bool SyntaxParserPhase1::parse_close_brace( XASM::CTokenStream &token_stream,
                                            shared_ptr<XASM::SToken> &token_ptr )
{
    // �Ҵ�����ֻ���ں�����
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_char_expected_error('}');
        return false;
		}

		CFunctionTable::Instance()->set_func_info(SyntaxVarible::Instance()->m_curr_func_name,
                                              SyntaxVarible::Instance()->m_curr_func_param_count,
                                              SyntaxVarible::Instance()->m_curr_func_local_data_size);

    // ���������
		SyntaxVarible::Instance()->m_is_func_active = false;
		return true;
}

// ����ָ��, �˴�ֻͳ��ָ������
bool SyntaxParserPhase1::parse_instruction( XASM::CTokenStream &token_stream,
                                            shared_ptr<XASM::SToken> &token_ptr )
{
    // ָ������ں�����
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_INSTR);
        return false;
		}

		++SyntaxVarible::Instance()->m_instr_stream_size;
		return true;
}

// ����SET_STACKSIZE ����
bool SyntaxParserPhase1::parse_stacksize( XASM::CTokenStream &token_stream,
                                          shared_ptr<XASM::SToken> &token_ptr )
{
    // �����ں�����
		if (SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_LOCAL_SETSTACKSIZE);
        return false;
		}

    // �����ظ�����
		if (SyntaxVarible::Instance()->m_is_stacksize_found)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_MULTIPLE_SETSTACKSIZES);
        return false;
		}

    // ����������������ֵ
		auto next_token = token_stream.peek_next_token();
		if (TOKEN_TYPE_INT != next_token->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_STACK_SIZE);
        return false;
		}

		SyntaxVarible::Instance()->m_header.stack_size = atoi(token_ptr->lexeme.c_str());
		SyntaxVarible::Instance()->m_is_stacksize_found = true;
		return true;
}

// �����������������
bool SyntaxParserPhase1::parse_var( XASM::CTokenStream &token_stream,
                                    shared_ptr<XASM::SToken> &token_ptr )
{
    // �����������Ǳ�ʶ��
		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
        return false;
		}

    // ������һ�����Է����ִ�����Ϊ������ var v ���� var v[10]
		int size = 1;    // ������������Ϊ������Ϊ����Ԫ�ظ�������������Ϊ1
		auto next_token = token_stream.peek_next_token();
		if (TOKEN_TYPE_OPEN_BRACKET == next_token->type)
		{
        // ������������ţ����ʾ�Ƕ�������
        next_token = token_stream.next_token();
        next_token = token_stream.next_token();

        // ����������, �Ҵ���0
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

        // ���������������
        next_token = token_stream.next_token();
        if (TOKEN_TYPE_CLOSE_BRACKET != next_token->type)
        {
            CErrorReporter::Instance()->exit_on_char_expected_error(']');
            return false;
        }
		}

    // ȫ�ֱ����;ֲ������������㷽ʽ��ͬ
		int stack_index = SyntaxVarible::Instance()->m_is_func_active ? -(SyntaxVarible::Instance()->m_curr_func_local_data_size + 2) 
                      : SyntaxVarible::Instance()->m_header.global_data_size;

		if (-1 == CSymbolTable::Instance()->add(token_ptr->lexeme,
                                            size,
                                            stack_index,
                                            SyntaxVarible::Instance()->m_curr_func_index))
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_REDEFINITION);
        return false;
		}

    // ����index����
		if (SyntaxVarible::Instance()->m_is_func_active)
		{
        SyntaxVarible::Instance()->m_curr_func_local_data_size += size;
		}
		else
		{
        SyntaxVarible::Instance()->m_header.global_data_size += size;
		}
		return true;
}

// �����������������
bool SyntaxParserPhase1::parse_func( XASM::CTokenStream &token_stream,
                                     shared_ptr<XASM::SToken> &token_ptr )
{
    // �����ں����ж��庯��
		if (SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_NESTED_FUNC);
        return false;
		}

    // ����Ϊ��ʶ��
		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
        return false;
		}

    // ��ӵ�������
		string str_name = token_ptr->lexeme;
		int entry_point = SyntaxVarible::Instance()->m_instr_stream_size;
		int func_index = CFunctionTable::Instance()->add(str_name, entry_point);
		if (-1 == func_index)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_FUNC_REDEFINITION);
        return false;
		}

    // �Ƿ�Ϊ������, �˴����ÿ����ظ��������������ظ���ػ����ʧ��
		if (MAIN_FUNC_NAME == str_name)
		{
        SyntaxVarible::Instance()->m_header.is_mainfunc_present = true;
        SyntaxVarible::Instance()->m_header.mainfunc_index = func_index;
		}

		SyntaxVarible::Instance()->m_is_func_active = true;
    SyntaxVarible::Instance()->m_curr_func_name = str_name;
    SyntaxVarible::Instance()->m_curr_func_index = func_index;
  	SyntaxVarible::Instance()->m_curr_func_param_count = 0;
  	SyntaxVarible::Instance()->m_curr_func_local_data_size = 0;

    // ��ת����һ��
		token_ptr = token_stream.next_token();
		while (token_ptr && TOKEN_TYPE_NEWLINE == token_ptr->type)
		{
        token_ptr = token_stream.next_token();
		};

    // ����Ϊ�������
		if (!token_ptr || TOKEN_TYPE_OPEN_BRACE != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_char_expected_error('{');
        return false;
		}

		// ��FUNC�Ľ�����Զ����RETָ����Դ˴���Ҫ++
		++SyntaxVarible::Instance()->m_instr_stream_size;
		return true;
}

// ������������ָ��
bool SyntaxParserPhase1::parse_param( XASM::CTokenStream &token_stream,
                                      shared_ptr<XASM::SToken> &token_ptr )
{
    // �����ں�����
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_PARAM);
		}

    // �������������ж������
		if (SyntaxVarible::Instance()->m_curr_func_name == MAIN_FUNC_NAME)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_MAIN_PARAM);
		}

    // ����Ϊ��ʶ��
		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
        return false;
		}

		++SyntaxVarible::Instance()->m_curr_func_param_count;
		return true;
}

// Ĭ�Ͻ�������
bool SyntaxParserPhase1::parse_default( XASM::CTokenStream &token_stream,
                                        shared_ptr<XASM::SToken> &token_ptr )
{
    // �����ǻ������Է�
		if (TOKEN_TYPE_NEWLINE != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_INPUT);
        return false;
		}

		return true;
}

// ���ݵ�ǰ���Է�������Ӧ�Ľ�������, ��û���򷵻�null
SyntaxParserPhase2::ParseFuncType SyntaxParserPhase2::get_parse_func(ETokenType token_type) const
{

    using TokenFuncMap = map<ETokenType, ParseFuncType>;
    static TokenFuncMap token_func_map = {
        {
            TOKEN_TYPE_CLOSE_BRACE,
            bind(&SyntaxParserPhase2::parse_close_brace, this, _1, _2, _3)
        },
        {
            TOKEN_TYPE_INSTRUCTION,
            bind(&SyntaxParserPhase2::parse_instruction, this, _1, _2, _3)
        },
        {
            TOKEN_TYPE_FUNC,
            bind(&SyntaxParserPhase2::parse_func, this, _1, _2, _3)
        },
        {
            TOKEN_TYPE_PARAM,
            bind(&SyntaxParserPhase2::parse_param, this, _1, _2, _3)
        }
    };

    auto map_itor = token_func_map.find(token_type);
    if (token_func_map.end() != map_itor)
    {
        return map_itor->second;
    }

    return ParseFuncType();
}

bool SyntaxParserPhase2::parse( XASM::CTokenStream &token_stream,
                                XASM::CInstrStream &instr_stream )
{
    SyntaxVarible::Instance()->reset();
    token_stream.reset();

		auto token_ptr = token_stream.next_token();
		while (token_ptr)
		{
        if (TOKEN_TYPE_END_OF_STREAM == token_ptr->type)
        {
            return true;
        }

        auto functor = get_parse_func(token_ptr->type);
        if (functor && (!functor(token_stream, token_ptr, instr_stream)))
        {
            return false;
        }

        /// �˴���Ҫ������ת����һ��....���ǲ�����~~~
        /// ��ʱ������ת����һ�н��в�������ÿ�н�β����һ��TOKEN_TYPE_NEWLINE
        while (TOKEN_TYPE_NEWLINE != token_ptr->type)
        {
            token_ptr = token_stream.next_token();
        }

        token_ptr = token_stream.next_token();
		}

		return true;
}

bool SyntaxParserPhase2::parse_func( XASM::CTokenStream &token_stream,
                                     shared_ptr<XASM::SToken> &token_ptr,
                                     XASM::CInstrStream &instr_stream )
{
    // ��ȡ��һ��token, ��������
    token_ptr = token_stream.next_token();
    auto&& func_ptr = CFunctionTable::Instance()->get_func_by_name(token_ptr->lexeme);

    SyntaxVarible::Instance()->m_is_func_active = true;
    SyntaxVarible::Instance()->m_curr_func_param_count = 0;
    SyntaxVarible::Instance()->m_curr_func_index = func_ptr->index;
    SyntaxVarible::Instance()->m_curr_func_name = func_ptr->name;
    SyntaxVarible::Instance()->m_curr_func_local_data_size = func_ptr->localdata_size;

    token_ptr = token_stream.next_token();
    while(token_ptr->type == TOKEN_TYPE_NEWLINE)
    {
        token_ptr = token_stream.next_token();
    }

    return true;
}

bool SyntaxParserPhase2::parse_param( XASM::CTokenStream &token_stream,
                                      shared_ptr<XASM::SToken> &token_ptr,
                                      XASM::CInstrStream &instr_stream )
{
    token_ptr = token_stream.next_token();
    if (token_ptr->type != TOKEN_TYPE_IDENTIFY)
    {
        CErrorReporter::Instance()->exit_on_code_error(ERROR_MSSG_IDENT_EXPECTED, token_ptr->row, token_ptr->file_name.c_str());
    }

    int stack_index = -(SyntaxVarible::Instance()->m_curr_func_local_data_size + 2 + (SyntaxVarible::Instance()->m_curr_func_param_count + 1));

    if (-1 == CSymbolTable::Instance()->add(token_ptr->lexeme,
                                            1,
                                            stack_index,
                                            SyntaxVarible::Instance()->m_curr_func_index))
    {
        CErrorReporter::Instance()->exit_on_code_error(ERROR_MSSG_IDENT_REDEFINITION,token_ptr->row, token_ptr->file_name.c_str());
    }

    ++SyntaxVarible::Instance()->m_curr_func_param_count;

    return true;
}

bool SyntaxParserPhase2::parse_close_brace( XASM::CTokenStream &token_stream,
                                            shared_ptr<XASM::SToken> &token_ptr,
                                            XASM::CInstrStream &instr_stream )
{
    SyntaxVarible::Instance()->m_is_func_active = false;
    if (SyntaxVarible::Instance()->m_curr_func_name == MAIN_FUNC_NAME)
    {
        SInstr ins;
        ins.op_code = INSTR_EXIT;
        ins.op_count = 1;

        SOperand op;
        op.type = OP_FLAG_TYPE_INT;
        op.int_literal = 0;
        ins.ops.push_back(op);

        instr_stream.push_back(ins);
    }
    else
    {
        SInstr ins;
        ins.op_code = INSTR_RET;
        ins.op_count = 0;

        instr_stream.push_back(ins);
    }

    return true;
}

bool SyntaxParserPhase2::parse_instruction( XASM::CTokenStream &token_stream, shared_ptr<XASM::SToken> &token_ptr,
                                            XASM::CInstrStream &instr_stream )
{
    auto instr_ptr = CInstrLookupTable::Instance()->get_instr_by_mnemonic(token_ptr->lexeme);
    if (!instr_ptr){
        return false;
    }

    SInstr ins;
    ins.op_code = instr_ptr->op_code;
    ins.op_count = instr_ptr->op_count;

    for (int index = 0; index < ins.op_count; ++index)
    {
        OpTypes op_type = instr_ptr->ops[index];

        token_ptr = token_stream.next_token();
        switch(token_ptr->type)
        {
            case TOKEN_TYPE_INT:
                if (op_type & OP_FLAG_TYPE_INT)
                {
                    SOperand op;
                    op.type = OP_FLAG_TYPE_INT;
                    op.int_literal = stoi(token_ptr->lexeme);
                    ins.ops.push_back(op);
                }
                else
                {
                    CErrorReporter::Instance()->exit_on_code_error(ERROR_MSSG_INVALID_OP,
                                                                   token_ptr->row,
                                                                   token_ptr->file_name.c_str());
                }
                break;
        }
    }

    return true;
}

}

/// ����﷨�����ܲ���׳
void CSyntaxParser::parse(CTokenStream& token_stream)
{
		token_stream.reset();
		if (!detail::SyntaxParserPhase1().parse(token_stream))
		{
        return;
		}

		token_stream.reset();
    CInstrStream instr_stream;
		if (!detail::SyntaxParserPhase2().parse(token_stream, instr_stream))
		{
        return;
		}
}

}
