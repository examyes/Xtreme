#pragma once

#include <list>
#include <string>
#include <memory>
using std::list;
using std::string;
using std::shared_ptr;

#include "../utils/utils.h"
#include "PublicDefine.h"

namespace XASM
{
	class CSymbolTable : public Singleton<CSymbolTable>
	{
		friend class Singleton<CSymbolTable>;
	private:
		list<shared_ptr<SSymbolNode>> m_table;

	public:
		shared_ptr<SSymbolNode> get_symbol_by_ident(string& val_ident, int func_index);
		int get_stack_index_by_ident(string& val_ident, int func_index);
		int get_size_by_ident(string& val_ident, int func_index);

		int add(string& val_ident, int size, int stack_index, int func_index);

	private:
		CSymbolTable(){};
		~CSymbolTable(){};
	};
}


