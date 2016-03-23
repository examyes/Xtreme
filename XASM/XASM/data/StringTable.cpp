#include "StringTable.h"

#include <algorithm>

namespace XASM
{
	int CStringTable::add(std::string& val_str)
	{
		list<string>::iterator itor = m_table.begin();
		int curr_index = 0;
		while (m_table.end() != itor)
		{
			if (*itor == val_str)
			{
				return curr_index;
			}

			++curr_index;
			++itor;
		}

		m_table.push_back(val_str);
		return curr_index;
	}
}

