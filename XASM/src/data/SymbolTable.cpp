#include "SymbolTable.h"

#include <algorithm>

namespace XASM
{

shared_ptr<SSymbolNode> CSymbolTable::get_symbol_by_ident(string& val_ident, int func_index)
{
		auto find_functor = [&](shared_ptr<SSymbolNode>& node_ptr){
        if (val_ident == node_ptr->ident)
        {
            if (node_ptr->func_index == func_index || 
                node_ptr->stack_index >= 0)
            {
                return true;
            }
        }

        return false;
		};

		auto itor = std::find_if(m_table.begin(), m_table.end(), find_functor);
		if (m_table.end() != itor)
		{
        return *itor;
		}

		return shared_ptr<SSymbolNode>(NULL);
}

int CSymbolTable::get_size_by_ident(string& val_ident, int func_index)
{
		return get_symbol_by_ident(val_ident, func_index)->size;
}

int CSymbolTable::get_stack_index_by_ident(string& val_ident, int func_index)
{
		return get_symbol_by_ident(val_ident, func_index)->stack_index;
}

int CSymbolTable::add(string& val_ident, int size, int stack_index, int func_index)
{
		if (get_symbol_by_ident(val_ident, func_index))
		{
        return -1;
		}

		auto node_ptr = std::make_shared<SSymbolNode>();
		node_ptr->ident = val_ident;
		node_ptr->size = size;
		node_ptr->stack_index = stack_index;
		node_ptr->func_index = func_index;

		m_table.push_back(node_ptr);
		node_ptr->index = m_table.size() - 1;

		return node_ptr->index;
}

}
