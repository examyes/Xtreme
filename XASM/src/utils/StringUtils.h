#pragma once

#include <string>
using std::string;

namespace XASM
{

/// �Ƿ��Ǳ�ʶ���ַ�
bool ident_p(char val_ch);

/// �Ƿ�ָ���
bool delimiter_p(char val_ch);

/// ��ʶ��
bool ident_p(const string& val_str);

}

