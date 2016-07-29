#pragma once

#include <liter/utils/Singleton.h>


#define ERROR_MSSG_INVALID_INPUT	\
	"Invalid input"

#define ERROR_MSSG_LOCAL_SETSTACKSIZE	\
	"SetStackSize can only appear in the global scope"

#define ERROR_MSSG_INVALID_STACK_SIZE	\
	"Invalid stack size"

#define ERROR_MSSG_MULTIPLE_SETSTACKSIZES	\
	"Multiple instances of SetStackSize illegal"

#define ERROR_MSSG_IDENT_EXPECTED	\
	"Identifier expected"

#define ERROR_MSSG_INVALID_ARRAY_SIZE	\
	"Invalid array size"

#define ERROR_MSSG_IDENT_REDEFINITION	\
	"Identifier redefinition"

#define ERROR_MSSG_UNDEFINED_IDENT	\
	"Undefined identifier"

#define ERROR_MSSG_NESTED_FUNC	\
	"Nested functions illegal"

#define ERROR_MSSG_FUNC_REDEFINITION	\
	"Function redefinition"

#define ERROR_MSSG_UNDEFINED_FUNC	\
	"Undefined function"

#define ERROR_MSSG_GLOBAL_PARAM	\
	"Parameters can only appear inside functions"

#define ERROR_MSSG_MAIN_PARAM	\
	"_Main () function cannot accept parameters"

#define ERROR_MSSG_GLOBAL_LINE_LABEL	\
	"Line labels can only appear inside functions"

#define ERROR_MSSG_LINE_LABEL_REDEFINITION	\
	"Line label redefinition"

#define ERROR_MSSG_UNDEFINED_LINE_LABEL	\
	"Undefined line label"

#define ERROR_MSSG_GLOBAL_INSTR	\
	"Instructions can only appear inside functions"

#define ERROR_MSSG_INVALID_INSTR	\
	"Invalid instruction"

#define ERROR_MSSG_INVALID_OP	\
	"Invalid operand"

#define ERROR_MSSG_INVALID_STRING	\
	"Invalid string"

#define ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED	\
	"Arrays must be indexed"

#define ERROR_MSSG_INVALID_ARRAY	\
	"Invalid array"

#define ERROR_MSSG_INVALID_ARRAY_INDEX	\
	"Invalid array index"

namespace XASM
{

class CErrorReporter : public liter::Singleton<CErrorReporter>
{
		friend class Singleton<CErrorReporter>;
public:
		void exit_on_error(const char* msg);
		void exit_on_code_error(const char* msg, int row, const char* src);
		void exit_on_char_expected_error(char ch);

private:
		CErrorReporter() {};
		~CErrorReporter() {};
};

}


