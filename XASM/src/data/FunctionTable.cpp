#include "FunctionTable.h"

#include <algorithm>

namespace XASM
{

shared_ptr<SFuncNode> CFunctionTable::get_func_by_name(string& val_name)
{
		auto find_functor = [&val_name](shared_ptr<SFuncNode>& node_ptr){
			return (node_ptr)->name == val_name;
		};

		auto itor = std::find_if(m_table.begin(), m_table.end(), find_functor);
		if (m_table.end() != itor)
		{
			return *itor;
		}

		return shared_ptr<SFuncNode>(NULL);
}

void CFunctionTable::set_func_info(string& val_name, int param_count, int localdata_size)
{
		shared_ptr<SFuncNode> node_ptr = get_func_by_name(val_name);
		node_ptr->param_count = param_count;
		node_ptr->localdata_size = localdata_size;
}

int CFunctionTable::add(string& val_name, int entry_point)
{
		if (get_func_by_name(val_name))
		{
			return -1;
		}

		auto node_ptr = std::make_shared<SFuncNode>();
		node_ptr->name = val_name;
		node_ptr->entry_point = entry_point;

		m_table.push_back(node_ptr);
		node_ptr->index = m_table.size() - 1;

		return node_ptr->index;
}

}
