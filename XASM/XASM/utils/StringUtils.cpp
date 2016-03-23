#include "StringUtils.h"

#include <cctype>
#include <functional>
#include <array>
#include <algorithm>

namespace XASM
{
	bool is_char_numeric(char val_ch)
	{
		return isdigit(val_ch);
	}

	bool is_char_whitespace(char val_ch)
	{
		return isspace(val_ch);
	}

	bool is_char_ident(char val_ch)
	{
		return isalnum(val_ch) || 
			'_' == val_ch;
	}

	bool is_char_delimiter(char val_ch)
	{
		return is_char_whitespace(val_ch) ||
			':' == val_ch ||
			',' == val_ch ||
			'"' == val_ch ||
			'[' == val_ch ||
			']' == val_ch ||
			'{' == val_ch ||
			'}' == val_ch;
	}

	bool is_string_int(const string& val_str)
	{
		struct SStatFunc
		{
			std::function<bool(char, int&)> stat_func;
		};

		std::array<SStatFunc, 3> stat_funcs;
		stat_funcs[0] = {[](char val_ch, int& status) -> bool{
			if ('-' == val_ch)
			{
				status = 1;
			}
			else if (is_char_numeric(val_ch))
			{
				status = 2;
			}
			else
			{
				return false;
			}

			return true;
		} };
		stat_funcs[1] = {[](char val_ch, int& status) -> bool {
			if (!is_char_numeric(val_ch))
			{
				return false;
			}

			status = 2;
			return true;
		} };
		stat_funcs[2] = {[](char val_ch, int& status) -> bool {
			if (!is_char_numeric(val_ch))
			{
				return false;
			}
			return true;
		} };

		int status = 0;
		for (auto&& val_ch : val_str)
		{
			if (!stat_funcs[status].stat_func(val_ch, status))
			{
				return false;
			}
		}

		return 2 == status;
	}

	bool is_string_float(const string& val_str)
	{
		struct SStatFunc
		{
			std::function<bool(char, int&)> stat_func;
		};

		std::array<SStatFunc, 5> stat_funcs;
		stat_funcs[0] = { [](char val_ch, int& status) -> bool {
			if ('-' == val_ch)
			{
				status = 1;
			}
			else if (is_char_numeric(val_ch))
			{
				status = 2;
			}
			else
			{
				return false;
			}

			return true;
		} };
		stat_funcs[1] = { [](char val_ch, int& status) -> bool {
			if (!is_char_numeric(val_ch))
			{
				return false;
			}

			status = 2;
			return true;
		} };
		stat_funcs[2] = { [](char val_ch, int& status) -> bool {
			if ('.' == val_ch)
			{
				status = 3;
			}
			else if (!is_char_numeric(val_ch))
			{
				return false;
			}

			return true;
		} };
		stat_funcs[3] = { [](char val_ch, int& status) -> bool {
			if (!is_char_numeric(val_ch))
			{
				return false;
			}

			status = 4;
			return true;
		} };
		stat_funcs[4] = { [](char val_ch, int& status) -> bool {
			return is_char_numeric(val_ch);
		} };

		int status = 0;
		for (auto&& val_ch : val_str)
		{
			if (!stat_funcs[status].stat_func(val_ch, status))
			{
				return false;
			}
		}

		return 2 == status || 
			4 == status;
	}

	bool is_string_whitespace(const string& val_str)
	{
		for (auto&& val_ch : val_str)
		{
			if (!is_char_whitespace(val_ch))
			{
				return false;
			}
		}

		return true;
	}

	bool is_string_ident(const string& val_str)
	{
		struct SStatFunc
		{
			std::function<bool(char, int&)> stat_func;
		};

		std::array<SStatFunc, 2> stat_funcs;
		stat_funcs[0] = { [](char val_ch, int& status) -> bool {
			if (is_char_ident(val_ch) && (!is_char_numeric(val_ch)))
			{
				status = 1;
				return true;
			}

			return false;
		} };
		stat_funcs[1] = { [](char val_ch, int& status) -> bool {
			return is_char_ident(val_ch);
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

	string trim(const string& val_str)
	{
		return trim_left(trim_right(val_str));
	}

	string trim_left(const string& val_str)
	{
		auto itor = std::find_if(val_str.begin(), 
			val_str.end(), 
			std::not1(std::function<bool(char)>(is_char_whitespace)));
		return string(itor, val_str.end());
	}

	string trim_right(const string& val_str)
	{
		auto itor = std::find_if(val_str.rbegin(), 
			val_str.rend(), 
			std::not1(std::function<bool(char)>(is_char_whitespace)));
		return string(val_str.begin(), itor.base());
	}
}