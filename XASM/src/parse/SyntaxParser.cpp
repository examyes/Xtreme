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

// 根据当前属性符返回相应的解析函数
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

// 解析函数，解析整个属性符流
bool SyntaxParserPhase1::parse( XASM::CTokenStream &token_stream )
{
    SyntaxVarible::Instance()->reset();
    token_stream.reset();

    auto token_ptr = token_stream.next_token();
		while (token_ptr)
		{
        // 到达属性符流末尾
        if (TOKEN_TYPE_END_OF_STREAM == token_ptr->type)
        {
            return true;
        }

        // 针对不同的属性符调用不同的处理函数
        auto functor = get_parse_func(token_ptr->type);
        if (!functor(token_stream, token_ptr))
        {
            return false;
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

// 解析标识符属性, 此处的标识符是作为标号的，作为变量的标识符应该在其他指令中处理
bool SyntaxParserPhase1::parse_identify( XASM::CTokenStream &token_stream,
                                         shared_ptr<XASM::SToken> &token_ptr )
{
		auto next_token = token_stream.next_token();

    // 标识符后必须跟随冒号
		if (TOKEN_TYPE_COLON != next_token->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_INSTR);
        return false;
		}

    // 标号必须在函数中
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_LINE_LABEL);
        return false;
		}

    // 添加到标号表中
		if (-1 == CLabelTable::Instance()->add(token_ptr->lexeme,
                                           SyntaxVarible::Instance()->m_instr_stream_size - 1,
                                           SyntaxVarible::Instance()->m_curr_func_index))
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_LINE_LABEL_REDEFINITION);
        return false;
		}

		return true;
}

// 解析右大括号
bool SyntaxParserPhase1::parse_close_brace( XASM::CTokenStream &token_stream,
                                            shared_ptr<XASM::SToken> &token_ptr )
{
    // 右大括号只用于函数体
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_char_expected_error('}');
        return false;
		}

		CFunctionTable::Instance()->set_func_info(SyntaxVarible::Instance()->m_curr_func_name,
                                              SyntaxVarible::Instance()->m_curr_func_param_count,
                                              SyntaxVarible::Instance()->m_curr_func_local_data_size);

    // 函数体结束
		SyntaxVarible::Instance()->m_is_func_active = false;
		return true;
}

// 解析指令, 此处只统计指令数量
bool SyntaxParserPhase1::parse_instruction( XASM::CTokenStream &token_stream,
                                            shared_ptr<XASM::SToken> &token_ptr )
{
    // 指令必须在函数中
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_INSTR);
        return false;
		}

		++SyntaxVarible::Instance()->m_instr_stream_size;
		return true;
}

// 解析SET_STACKSIZE 命令
bool SyntaxParserPhase1::parse_stacksize( XASM::CTokenStream &token_stream,
                                          shared_ptr<XASM::SToken> &token_ptr )
{
    // 不能在函数中
		if (SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_LOCAL_SETSTACKSIZE);
        return false;
		}

    // 不能重复出现
		if (SyntaxVarible::Instance()->m_is_stacksize_found)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_MULTIPLE_SETSTACKSIZES);
        return false;
		}

    // 接下来必须是整数值
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

// 解析变量定义操作符
bool SyntaxParserPhase1::parse_var( XASM::CTokenStream &token_stream,
                                    shared_ptr<XASM::SToken> &token_ptr )
{
    // 接下来必须是标识符
		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
        return false;
		}

    // 根据下一个属性符区分处理，因为可能是 var v 或者 var v[10]
		int size = 1;    // 变量个数，若为数组则为数组元素个数，单个变量为1
		auto next_token = token_stream.peek_next_token();
		if (TOKEN_TYPE_OPEN_BRACKET == next_token->type)
		{
        // 如果是左中括号，则表示是定义数组
        next_token = token_stream.next_token();
        next_token = token_stream.next_token();

        // 必须是整数, 且大于0
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

        // 必须紧跟右中括号
        next_token = token_stream.next_token();
        if (TOKEN_TYPE_CLOSE_BRACKET != next_token->type)
        {
            CErrorReporter::Instance()->exit_on_char_expected_error(']');
            return false;
        }
		}

    // 全局变量和局部变量索引计算方式不同
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

    // 增加index计数
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

// 解析函数定义操作符
bool SyntaxParserPhase1::parse_func( XASM::CTokenStream &token_stream,
                                     shared_ptr<XASM::SToken> &token_ptr )
{
    // 不能在函数中定义函数
		if (SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_NESTED_FUNC);
        return false;
		}

    // 必须为标识符
		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
        return false;
		}

    // 添加到函数表
		string str_name = token_ptr->lexeme;
		int entry_point = SyntaxVarible::Instance()->m_instr_stream_size;
		int func_index = CFunctionTable::Instance()->add(str_name, entry_point);
		if (-1 == func_index)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_FUNC_REDEFINITION);
        return false;
		}

    // 是否为主函数, 此处不用考虑重复主函数，如若重复则必会添加失败
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

    // 跳转到下一行
		token_ptr = token_stream.next_token();
		while (token_ptr && TOKEN_TYPE_NEWLINE == token_ptr->type)
		{
        token_ptr = token_stream.next_token();
		};

    // 必须为左大括号
		if (!token_ptr || TOKEN_TYPE_OPEN_BRACE != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_char_expected_error('{');
        return false;
		}

		// 在FUNC的结果会自动添加RET指令，所以此处需要++
		++SyntaxVarible::Instance()->m_instr_stream_size;
		return true;
}

// 解析函数参数指令
bool SyntaxParserPhase1::parse_param( XASM::CTokenStream &token_stream,
                                      shared_ptr<XASM::SToken> &token_ptr )
{
    // 必须在函数中
		if (!SyntaxVarible::Instance()->m_is_func_active)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_GLOBAL_PARAM);
		}

    // 不能在主函数中定义参数
		if (SyntaxVarible::Instance()->m_curr_func_name == MAIN_FUNC_NAME)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_MAIN_PARAM);
		}

    // 必须为标识符
		token_ptr = token_stream.next_token();
		if (TOKEN_TYPE_IDENTIFY != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_IDENT_EXPECTED);
        return false;
		}

		++SyntaxVarible::Instance()->m_curr_func_param_count;
		return true;
}

// 默认解析函数
bool SyntaxParserPhase1::parse_default( XASM::CTokenStream &token_stream,
                                        shared_ptr<XASM::SToken> &token_ptr )
{
    // 必须是换行属性符
		if (TOKEN_TYPE_NEWLINE != token_ptr->type)
		{
        CErrorReporter::Instance()->exit_on_error(ERROR_MSSG_INVALID_INPUT);
        return false;
		}

		return true;
}

// 根据当前属性符返回相应的解析函数, 如没有则返回null
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

bool SyntaxParserPhase2::parse_func( XASM::CTokenStream &token_stream,
                                     shared_ptr<XASM::SToken> &token_ptr,
                                     XASM::CInstrStream &instr_stream )
{
    // 获取下一个token, 即函数名
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

/// 这个语法分析很不健壮
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
