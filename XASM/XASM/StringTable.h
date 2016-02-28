#pragma once

#include <list>
#include <string>
using std::list;
using std::string;

#include "utils.h"

namespace XASM
{
	class CStringTable : Singleton<CStringTable>
	{
		friend class Singleton<CStringTable>;

	private:
		list<string> m_table;

	public:

		int add(std::string& val_str);

	private:
		CStringTable(){};
		~CStringTable(){};
	};
}


