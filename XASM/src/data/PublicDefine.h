#pragma once

#include <list>
#include <vector>
#include <string>
using std::list;
using std::vector;
using std::string;


namespace XASM
{

/// Token��ö��ֵ
enum ETokenType
{
		TOKEN_TYPE_INT = 1,					// �������泣��
		TOKEN_TYPE_FLOAT = 2,				// ���������泣��
		TOKEN_TYPE_STRING = 3,				// �ַ��������泣��
		TOKEN_TYPE_QUOTE = 4,				// ˫����
		TOKEN_TYPE_IDENTIFY = 5,			// ��ʶ��
		TOKEN_TYPE_COLON = 6,				// ð��
		TOKEN_TYPE_OPEN_BRACKET = 7,		// ��������
		TOKEN_TYPE_CLOSE_BRACKET = 8,		// ��������
		TOKEN_TYPE_COMMA = 9,				// ����
		TOKEN_TYPE_OPEN_BRACE = 10,			// �������
		TOKEN_TYPE_CLOSE_BRACE = 11,		// �Ҵ�����
		TOKEN_TYPE_NEWLINE = 12,			// ����
		TOKEN_TYPE_INSTRUCTION = 13,		// ָ��
		TOKEN_TYPE_SET_STACKSIZE = 14,		// SetStackSizeָʾ��
		TOKEN_TYPE_VAR = 15,				// Varָʾ��
		TOKEN_TYPE_FUNC = 16,				// Funcָʾ��
		TOKEN_TYPE_PARAM = 17,				// Paramָʾ��
		TOKEN_TYPE_REG_RETVAL = 18,			// _RetVal�Ĵ���
		TOKEN_TYPE_INVALID = 19,			// �����������ֵĴ������
		TOKEN_TYPE_END_OF_STREAM = 20,		// �����ַ�����β��
};

/// Token�ṹ
struct SToken
{
		ETokenType type;
		size_t row;
		string lexeme;
		string file_name;
};

/// ������
struct SOperand
{
		int type;						// ����������
		union
		{
        int int_literal;			// ��������ֵ
        float float_literal;		// ����������ֵ
        int string_table_index;		// �ַ���������
        int stack_index;			// ��ջ����
        int instr_index;			// ָ������
        int func_index;				// ��������
        int host_apicall_index;		// ��Ӧ�ó���API��������
        int reg;					// �Ĵ�����
		};
		int offset_index;				// ƫ������������unionΪstack_indexʱ����
};

/// ָ��
struct SInstr
{
		int op_code;
		int op_count;
		list<SOperand> ops;
};

/// �ű�ͷ
struct SScriptHeader
{
		int stack_size;				// ��ջ��С
		int global_data_size;		// �ű���ȫ�����ݵĴ�С
		int is_mainfunc_present;	// _Main() �Ƿ����
		int mainfunc_index;			// _Main() ����������
};

/// ����
struct SFuncNode
{
		int index;					// ����
		string name;				// ����
		int entry_point;			// ��ڵ�
		int param_count;			// ��������
		int localdata_size;			// �ֲ����ݴ�С
};

/// ����
struct SSymbolNode
{
		int index;					// ����
		string ident;				// ��ʶ��
		int size;					// ��С������=1������=N
		int stack_index;			// ����ָ��Ķ�ջ����
		int func_index;				// �������ڵĺ�������
};

/// ��ǩ
struct SLabelNode
{
		int index;					// ����
		string ident;				// ��ʶ��
		int target_index;			// Ŀ��ָ�������
		int func_index;				// ��ǩ�����ĺ�������
};

typedef int OpTypes;
enum EOpType
{
		OP_FLAG_TYPE_INT = 0x00000001,				// ����������
		OP_FLAG_TYPE_FLOAT = 0x00000002,			// ������������
		OP_FLAG_TYPE_STRING = 0x00000004,			// �ַ���������
		OP_FLAG_TYPE_MEM_REF = 0x00000008,			// �ڴ����ã���������������
		OP_FLAG_TYPE_LINE_LABEL = 0x00000010,		// �б�ǩ��������תָ��
		OP_FLAG_TYPE_FUNC_NAME = 0x00000020,		// ������������Callָ��
		OP_FLAG_TYPE_HOST_API_CALL = 0x00000040,	// ��Ӧ�ó���API���ã�����CallHostָ��
		OP_FLAG_TYPE_REG = 0x00000080				// �Ĵ�������_RetVal
};

/// �����---����������
enum EAsmOpType
{
    OP_TYPE_INT = 0,                        // ����������
    OP_TYPE_FLOAT = 1,                      // ������������
    OP_TYPE_STRING_INDEX = 2,               // �ַ���������, �洢�ַ���������
    OP_TYPE_ABS_STACK_INDEX = 3,            // ջ�Ͼ�������ֵ
    OP_TYPE_REL_STACK_INDEX = 4,            // ջ���������ֵ
    OP_TYPE_INSTR_INDEX = 5,                // ָ������
    OP_TYPE_FUNC_INDEX = 6,                 // ����������
    OP_TYPE_HOST_API_CALL_INDEX = 7,        // HostAPI
    OP_TYPE_REG = 8                         // �Ĵ���
};

/// ����ָ��
struct SInstrLookup
{
		string mnemonic;				// ���Ƿ�
		int op_code;					// ������
		int op_count;					// ������
		vector<OpTypes> ops;			// �������б�
};

/// ����������
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
