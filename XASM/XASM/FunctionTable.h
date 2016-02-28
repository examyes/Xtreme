#pragma once

#include <list>
#include <string>
#include <memory>
using std::list;
using std::string;
using std::shared_ptr;

#include "utils.h"
#include "PublicDefine.h"

namespace XASM
{
	class CFunctionTable : public Singleton<CFunctionTable>
	{
		friend class Singleton<CFunctionTable>;
	private:
		list<shared_ptr<SFuncNode>> m_table;

	public:
		shared_ptr<SFuncNode> get_func_by_name(string& val_name);
		void set_func_info(string& val_name, int param_count, int localdata_size);
		int add(string& val_name, int entry_point);


	private:
		CFunctionTable(){};
		~CFunctionTable(){};
	};

	using CHostApiCallTable = CFunctionTable;
}


