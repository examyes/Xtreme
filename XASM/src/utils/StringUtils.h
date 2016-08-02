#pragma once

#include <string>
using std::string;

namespace XASM
{

/// 是否是标识符字符
bool ident_p(char val_ch);

/// 是否分隔符
bool delimiter_p(char val_ch);

/// 标识符
bool ident_p(const string& val_str);

}

