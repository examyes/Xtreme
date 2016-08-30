#pragma once

#include <list>
#include <vector>
#include <string>
using std::list;
using std::vector;
using std::string;


namespace XASM
{

/// Token的枚举值
enum ETokenType
{
		TOKEN_TYPE_INT = 1,					// 整形字面常量
		TOKEN_TYPE_FLOAT = 2,				// 浮点型字面常量
		TOKEN_TYPE_STRING = 3,				// 字符串型字面常量
		TOKEN_TYPE_QUOTE = 4,				// 双引号
		TOKEN_TYPE_IDENTIFY = 5,			// 标识符
		TOKEN_TYPE_COLON = 6,				// 冒号
		TOKEN_TYPE_OPEN_BRACKET = 7,		// 左中括号
		TOKEN_TYPE_CLOSE_BRACKET = 8,		// 右中括号
		TOKEN_TYPE_COMMA = 9,				// 逗号
		TOKEN_TYPE_OPEN_BRACE = 10,			// 左大括号
		TOKEN_TYPE_CLOSE_BRACE = 11,		// 右大括号
		TOKEN_TYPE_NEWLINE = 12,			// 换行
		TOKEN_TYPE_INSTRUCTION = 13,		// 指令
		TOKEN_TYPE_SET_STACKSIZE = 14,		// SetStackSize指示符
		TOKEN_TYPE_VAR = 15,				// Var指示符
		TOKEN_TYPE_FUNC = 16,				// Func指示符
		TOKEN_TYPE_PARAM = 17,				// Param指示符
		TOKEN_TYPE_REG_RETVAL = 18,			// _RetVal寄存器
		TOKEN_TYPE_INVALID = 19,			// 不符合属性字的错误编码
		TOKEN_TYPE_END_OF_STREAM = 20,		// 属性字符流的尾部
};

/// Token结构
struct SToken
{
		ETokenType type;
		size_t row;
		string lexeme;
		string file_name;
};

/// 操作数
struct SOperand
{
		int type;						// 操作数类型
		union
		{
        int int_literal;			// 整型字面值
        float float_literal;		// 浮点型字面值
        int string_table_index;		// 字符串表索引
        int stack_index;			// 堆栈索引
        int instr_index;			// 指令索引
        int func_index;				// 函数索引
        int host_apicall_index;		// 主应用程序API调用索引
        int reg;					// 寄存器码
		};
		int offset_index;				// 偏移量索引，当union为stack_index时可用
};

/// 指令
struct SInstr
{
		int op_code;
		int op_count;
		list<SOperand> ops;
};

/// 脚本头
struct SScriptHeader
{
		int stack_size;				// 堆栈大小
		int global_data_size;		// 脚本中全局数据的大小
		int is_mainfunc_present;	// _Main() 是否存在
		int mainfunc_index;			// _Main() 函数的索引
};

/// 函数
struct SFuncNode
{
		int index;					// 索引
		string name;				// 名称
		int entry_point;			// 入口点
		int param_count;			// 参数个数
		int localdata_size;			// 局部数据大小
};

/// 符号
struct SSymbolNode
{
		int index;					// 索引
		string ident;				// 标识符
		int size;					// 大小，变量=1，数组=N
		int stack_index;			// 符号指向的堆栈索引
		int func_index;				// 符号所在的函数索引
};

/// 标签
struct SLabelNode
{
		int index;					// 索引
		string ident;				// 标识符
		int target_index;			// 目标指令的索引
		int func_index;				// 标签所属的函数索引
};

typedef int OpTypes;
enum EOpType
{
		OP_FLAG_TYPE_INT = 0x00000001,				// 整形字面量
		OP_FLAG_TYPE_FLOAT = 0x00000002,			// 浮点型字面量
		OP_FLAG_TYPE_STRING = 0x00000004,			// 字符串字面量
		OP_FLAG_TYPE_MEM_REF = 0x00000008,			// 内存引用，变量或数组索引
		OP_FLAG_TYPE_LINE_LABEL = 0x00000010,		// 行标签，用于跳转指令
		OP_FLAG_TYPE_FUNC_NAME = 0x00000020,		// 函数名，用于Call指令
		OP_FLAG_TYPE_HOST_API_CALL = 0x00000040,	// 主应用程序API调用，用于CallHost指令
		OP_FLAG_TYPE_REG = 0x00000080				// 寄存器，即_RetVal
};

/// 汇编结果---操作数类型
enum EAsmOpType
{
    OP_TYPE_INT = 0,                        // 整形字面量
    OP_TYPE_FLOAT = 1,                      // 浮点型字面量
    OP_TYPE_STRING_INDEX = 2,               // 字符串字面量, 存储字符串表索引
    OP_TYPE_ABS_STACK_INDEX = 3,            // 栈上绝对索引值
    OP_TYPE_REL_STACK_INDEX = 4,            // 栈上相对索引值
    OP_TYPE_INSTR_INDEX = 5,                // 指令索引
    OP_TYPE_FUNC_INDEX = 6,                 // 函数表索引
    OP_TYPE_HOST_API_CALL_INDEX = 7,        // HostAPI
    OP_TYPE_REG = 8                         // 寄存器
};

/// 查找指令
struct SInstrLookup
{
		string mnemonic;				// 助记符
		int op_code;					// 操作码
		int op_count;					// 操作数
		vector<OpTypes> ops;			// 操作数列表
};

/// 操作符定义
enum EInstrOpcode
{
		INSTR_MOV			= 0,
		INSTR_ADD			= 1,
		INSTR_SUB			= 2,
		INSTR_MUL			= 3,
		INSTR_DIV			= 4,
		INSTR_MOD			= 5,
		INSTR_EXP			= 6,
		INSTR_NEG			= 7,
		INSTR_INC			= 8,
		INSTR_DEC			= 9,
		INSTR_AND			= 10,
		INSTR_OR			= 11,
		INSTR_XOR			= 12,
		INSTR_NOT			= 13,
		INSTR_SHL			= 14,
		INSTR_SHR			= 15,
		INSTR_CONCAT		= 16,
		INSTR_GETCHAR		= 17,
		INSTR_SETCHAR		= 18,
		INSTR_JMP			= 19,
		INSTR_JE			= 20,
		INSTR_JNE			= 21,
		INSTR_JG			= 22,
		INSTR_JL			= 23,
		INSTR_JGE			= 24,
		INSTR_JLE			= 25,
		INSTR_PUSH			= 26,
		INSTR_POP			= 27,
		INSTR_CALL			= 28,
		INSTR_RET			= 29,
		INSTR_CALLHOST		= 30,
		INSTR_PAUSE			= 31,
		INSTR_EXIT			= 32
};

#define MAIN_FUNC_NAME				"_Main"		// _Main ()'s name

}
