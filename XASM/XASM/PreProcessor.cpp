#include "PreProcessor.h"

#include <algorithm>

#include "StringUtils.h"

namespace XASM
{
	CSourceCodeHolder CPreProcessor::pre_process(CSourceCodeHolder& pre_holder)
	{
		CSourceCodeHolder holder;

		auto itor = pre_holder.begin();
		while (itor != pre_holder.end())
		{
			holder.append(pre_line((*itor)->text()), (*itor)->row());
			++itor;
		}

		return holder;
	}

	string CPreProcessor::pre_line(const string& val_str)
	{
		return trim(strip_comments(val_str));
	}

	string CPreProcessor::strip_comments(const string& val_str)
	{
		bool b_is_string = false;
		auto find_functor = [&b_is_string](char val_ch){
			if ('"' == val_ch)
			{
				b_is_string = !b_is_string;
			}
			else if (';' == val_ch)
			{
				if (!b_is_string)
				{
					return true;
				}
			}

			return false;
		};

		auto itor = std::find_if(val_str.begin(), val_str.end(), find_functor);
		return string(val_str.begin(), itor);
	}
}

