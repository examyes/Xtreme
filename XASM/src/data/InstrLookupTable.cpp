#include "InstrLookupTable.h"

#include <algorithm>

namespace XASM
{
	shared_ptr<SInstrLookup> CInstrLookupTable::get_instr_by_mnemonic(string& val_mnemonic)
	{
		auto find_functor = [&val_mnemonic](shared_ptr<SInstrLookup>& node_ptr){
			return node_ptr->mnemonic == val_mnemonic;
		};

		auto itor = std::find_if(m_table.begin(), m_table.end(), find_functor);
		if (m_table.end() != itor)
		{
			return *itor;
		}

		return shared_ptr<SInstrLookup>(NULL);
	}

	int CInstrLookupTable::add(string& val_mnemonic, int op_code, int op_count)
	{
		static int instr_index = 0;

		auto node_ptr = std::make_shared<SInstrLookup>();
		node_ptr->mnemonic = val_mnemonic;
		node_ptr->op_code = op_code;
		node_ptr->op_count = op_count;
		node_ptr->ops.resize(op_count);

		m_table.push_back(node_ptr);

		return instr_index++;
	}

	void CInstrLookupTable::set_op_types(int instr_index, int op_index, OpTypes op_type)
	{
		shared_ptr<SInstrLookup> inst_ptr = m_table.at(instr_index);
		inst_ptr->ops[op_index] = op_type;
	}

	void CInstrLookupTable::init_table()
	{
		/// 初始化指令查找表，定义XASM指令集
		int instr_index = 0;
		string mnemonic;

		// Mov Dest, Src
		mnemonic = "Mov";
		instr_index = add(mnemonic, INSTR_MOV, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Add Dest, Src
		mnemonic = "Add";
		instr_index = add(mnemonic, INSTR_ADD, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Sub Dest, Src
		mnemonic = "Sub";
		instr_index = add(mnemonic, INSTR_SUB, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Mul Dest, Src
		mnemonic = "Mul";
		instr_index = add(mnemonic, INSTR_MUL, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Div Dest, Src
		mnemonic = "Div";
		instr_index = add(mnemonic, INSTR_DIV, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Exp Dest, Src
		mnemonic = "Exp";
		instr_index = add(mnemonic, INSTR_EXP, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Neg Dest
		mnemonic = "Neg";
		instr_index = add(mnemonic, INSTR_NEG, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		
		// Inc Dest
		mnemonic = "Inc";
		instr_index = add(mnemonic, INSTR_INC, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Dec Dest
		mnemonic = "Dec";
		instr_index = add(mnemonic, INSTR_DEC, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// And Dest, Src
		mnemonic = "And";
		instr_index = add(mnemonic, INSTR_AND, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Or Dest, Src
		mnemonic = "Or";
		instr_index = add(mnemonic, INSTR_OR, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// XOr Dest, Src
		mnemonic = "XOr";
		instr_index = add(mnemonic, INSTR_XOR, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Not Dest
		mnemonic = "Not";
		instr_index = add(mnemonic, INSTR_NOT, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// ShL Dest, Src
		mnemonic = "ShL";
		instr_index = add(mnemonic, INSTR_SHL, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// ShR Dest, Src
		mnemonic = "ShR";
		instr_index = add(mnemonic, INSTR_SHR, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Concat Str0, Str1
		mnemonic = "Concat";
		instr_index = add(mnemonic, INSTR_CONCAT, 2);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// GetChar Dest, Src, Index
		mnemonic = "GetChar";
		instr_index = add(mnemonic, INSTR_GETCHAR, 3);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 2, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// SetChar Dest, Index, Src
		mnemonic = "SetChar";
		instr_index = add(mnemonic, INSTR_SETCHAR, 3);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 2, OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		
		// Jmp Label
		mnemonic = "Jmp";
		instr_index = add(mnemonic, INSTR_JMP, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_LINE_LABEL);

		// JE Op0, Op1, Label
		mnemonic = "JE";
		instr_index = add(mnemonic, INSTR_JE, 3);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JG Op0, Op1, Label
		mnemonic = "JG";
		instr_index = add(mnemonic, INSTR_JG, 3);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JL Op0, Op1, Label
		mnemonic = "JL";
		instr_index = add(mnemonic, INSTR_JL, 3);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JGE Op0, Op1, Label
		mnemonic = "JGE";
		instr_index = add(mnemonic, INSTR_JGE, 3);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 2, OP_FLAG_TYPE_LINE_LABEL);

		// JLE Op0, Op1, Label
		mnemonic = "JLE";
		instr_index = add(mnemonic, INSTR_JLE, 3);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 1, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
		set_op_types(instr_index, 2, OP_FLAG_TYPE_LINE_LABEL);

		// Push Src
		mnemonic = "Push";
		instr_index = add(mnemonic, INSTR_PUSH, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Pop Dest
		mnemonic = "Pop";
		instr_index = add(mnemonic, INSTR_POP, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Call FunctionName
		mnemonic = "Call";
		instr_index = add(mnemonic, INSTR_CALL, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_FUNC_NAME);

		// Ret
		mnemonic = "Ret";
		instr_index = add(mnemonic, INSTR_RET, 0);

		// CallHost FunctionName
		mnemonic = "CallHost";
		instr_index = add(mnemonic, INSTR_CALLHOST, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_HOST_API_CALL);

		// Pause Duration
		mnemonic = "Pause";
		instr_index = add(mnemonic, INSTR_PAUSE, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);

		// Exit Code
		mnemonic = "Exit";
		instr_index = add(mnemonic, INSTR_EXIT, 1);
		set_op_types(instr_index, 0, OP_FLAG_TYPE_INT |
			OP_FLAG_TYPE_FLOAT |
			OP_FLAG_TYPE_STRING |
			OP_FLAG_TYPE_MEM_REF |
			OP_FLAG_TYPE_REG);
	}
}
