#include "StringUtils.h"

#include <cctype>
#include <functional>
#include <array>
#include <algorithm>
#include <liter/utils/string_utils.h>

using namespace liter;

namespace XASM
{

bool ident_p(char val_ch)
{
		return isalnum(val_ch) ||
            '_' == val_ch;
}

bool delimiter_p(char val_ch)
{
		return whitespace_p(val_ch) ||
            ':' == val_ch ||
            ',' == val_ch ||
            '"' == val_ch ||
            '[' == val_ch ||
            ']' == val_ch ||
            '{' == val_ch ||
            '}' == val_ch;
}

bool ident_p(const string& val_str)
{
		struct SStatFunc
		{
        std::function<bool(char, int&)> stat_func;
		};

		std::array<SStatFunc, 2> stat_funcs;
		stat_funcs[0] = { [](char val_ch, int& status) -> bool {
            if (ident_p(val_ch) && (!numeric_p(val_ch)))
            {
                status = 1;
                return true;
            }

            return false;
        } };
		stat_funcs[1] = { [](char val_ch, int& status) -> bool {
            return ident_p(val_ch);
        } };

		int status = 0;
		for (auto&& val_ch : val_str)
		{
        if (!stat_funcs[status].stat_func(val_ch, status))
        {
            return false;
        }
		}

		return 1 == status;
}

}
