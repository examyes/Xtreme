#pragma once

#include <string>
using std::string;

namespace XASM
{
	/// 是否数字
	bool is_char_numeric(char val_ch);

	/// 是否空白
	bool is_char_whitespace(char val_ch);

	/// 是否是标识符字符
	bool is_char_ident(char val_ch);

	/// 是否分隔符
	bool is_char_delimiter(char val_ch);

	/// 是否整数
	bool is_string_int(const string& val_str);

	/// 浮点数
	bool is_string_float(const string& val_str);

	/// 空白
	bool is_string_whitespace(const string& val_str);

	/// 标识符
	bool is_string_ident(const string& val_str);

	/// 删除字符串的空白
	string trim(const string& val_str);
	string trim_left(const string& val_str);
	string trim_right(const string& val_str);
}

