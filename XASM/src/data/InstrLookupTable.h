#pragma once

#include <vector>
#include <string>
#include <memory>
using std::vector;
using std::string;
using std::shared_ptr;

#include "../utils/utils.h"
#include "PublicDefine.h"

namespace XASM
{
	class CInstrLookupTable : public Singleton<CInstrLookupTable>
	{
		friend class Singleton<CInstrLookupTable>;
	private:
		vector<shared_ptr<SInstrLookup>> m_table;

	public:
		shared_ptr<SInstrLookup> get_instr_by_mnemonic(string& val_mnemonic);

	private:
		CInstrLookupTable(){
			init_table();
		};

		~CInstrLookupTable(){};

		void init_table();
		int add(string& val_mnemonic, int op_code, int op_count);
		void set_op_types(int instr_index, int op_index, OpTypes op_type);
	};
}


