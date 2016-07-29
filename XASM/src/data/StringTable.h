#pragma once

#include <list>
#include <string>
using std::list;
using std::string;

#include <liter/utils/Singleton.h>


namespace XASM
{

class CStringTable : liter::Singleton<CStringTable>
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


