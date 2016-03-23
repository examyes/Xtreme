#pragma once

#include <string>
using std::string;

namespace XASM
{
	/// �Ƿ�����
	bool is_char_numeric(char val_ch);

	/// �Ƿ�հ�
	bool is_char_whitespace(char val_ch);

	/// �Ƿ��Ǳ�ʶ���ַ�
	bool is_char_ident(char val_ch);

	/// �Ƿ�ָ���
	bool is_char_delimiter(char val_ch);

	/// �Ƿ�����
	bool is_string_int(const string& val_str);

	/// ������
	bool is_string_float(const string& val_str);

	/// �հ�
	bool is_string_whitespace(const string& val_str);

	/// ��ʶ��
	bool is_string_ident(const string& val_str);

	/// ɾ���ַ����Ŀհ�
	string trim(const string& val_str);
	string trim_left(const string& val_str);
	string trim_right(const string& val_str);
}

