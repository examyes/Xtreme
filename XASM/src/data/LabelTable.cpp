#include "LabelTable.h"

#include <algorithm>

namespace XASM
{
	shared_ptr<SLabelNode> CLabelTable::get_label_by_ident(string& val_ident, int func_index)
	{
		auto find_functor = [&](shared_ptr<SLabelNode>& node_ptr){
			return node_ptr->ident == val_ident &&
				node_ptr->func_index == func_index;
		};

		auto itor = std::find_if(m_table.begin(), m_table.end(), find_functor);
		if (m_table.end() != itor)
		{
			return *itor;
		}

		return shared_ptr<SLabelNode>(NULL);
	}

	int CLabelTable::add(string& val_ident, int target_index, int func_index)
	{
		if (get_label_by_ident(val_ident, func_index))
		{
			return -1;
		}

		auto node_ptr = std::make_shared<SLabelNode>();
		node_ptr->ident = val_ident;
		node_ptr->func_index = func_index;
		node_ptr->target_index = target_index;

		m_table.push_back(node_ptr);
		node_ptr->index = m_table.size() - 1;

		return node_ptr->index;
	}
}
