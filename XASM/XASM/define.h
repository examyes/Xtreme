#pragma once

namespace XASM
{
	enum TokenType
	{
		TOKEN_TYPE_INT = 1,					//整形字面常量
		TOKEN_TYPE_FLOAT = 2,				//浮点型字面常量
		TOKEN_TYPE_STRING = 3,				//字符串型字面常量
		TOKEN_TYPE_QUOTE = 4,				//双引号
		TOKEN_TYPE_IDENTIFY = 5,			//标识符
		TOKEN_TYPE_COLON = 6,				//冒号
		TOKEN_TYPE_OPEN_BRACKET = 7,		//左中括号
		TOKEN_TYPE_CLOSE_BRACKET = 8,		//右中括号
		TOKEN_TYPE_COMMA = 9,				//逗号
		TOKEN_TYPE_OPEN_BRACE = 10,			//左大括号
		TOKEN_TYPE_CLOSE_BRACE = 11,		//右大括号
		TOKEN_TYPE_NEWLINE = 12,			//换行
		TOKEN_TYPE_INSTRUCTION = 13,		//指令
		TOKEN_TYPE_SET_STACKSIZE = 14,		//SetStackSize指示符
		TOKEN_TYPE_VAR = 15,				//Var指示符
		TOKEN_TYPE_FUNC = 16,				//Func指示符
		TOKEN_TYPE_PARAM = 17,				//Param指示符
		TOKEN_TYPE_REG_RETVAL = 18,			//_RetVal寄存器
		TOKEN_TYPE_INVALID = 19,			//不符合属性字的错误编码
		TOKEN_TYPE_END_OF_STREAM = 20,		//属性字符流的尾部
	};
}