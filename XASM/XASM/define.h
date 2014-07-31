#pragma once

namespace XASM
{
	enum TokenType
	{
		TOKEN_TYPE_INT = 1,					//�������泣��
		TOKEN_TYPE_FLOAT = 2,				//���������泣��
		TOKEN_TYPE_STRING = 3,				//�ַ��������泣��
		TOKEN_TYPE_QUOTE = 4,				//˫����
		TOKEN_TYPE_IDENTIFY = 5,			//��ʶ��
		TOKEN_TYPE_COLON = 6,				//ð��
		TOKEN_TYPE_OPEN_BRACKET = 7,		//��������
		TOKEN_TYPE_CLOSE_BRACKET = 8,		//��������
		TOKEN_TYPE_COMMA = 9,				//����
		TOKEN_TYPE_OPEN_BRACE = 10,			//�������
		TOKEN_TYPE_CLOSE_BRACE = 11,		//�Ҵ�����
		TOKEN_TYPE_NEWLINE = 12,			//����
		TOKEN_TYPE_INSTRUCTION = 13,		//ָ��
		TOKEN_TYPE_SET_STACKSIZE = 14,		//SetStackSizeָʾ��
		TOKEN_TYPE_VAR = 15,				//Varָʾ��
		TOKEN_TYPE_FUNC = 16,				//Funcָʾ��
		TOKEN_TYPE_PARAM = 17,				//Paramָʾ��
		TOKEN_TYPE_REG_RETVAL = 18,			//_RetVal�Ĵ���
		TOKEN_TYPE_INVALID = 19,			//�����������ֵĴ������
		TOKEN_TYPE_END_OF_STREAM = 20,		//�����ַ�����β��
	};
}