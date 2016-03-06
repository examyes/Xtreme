// XASM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "SourceCodeHolder.h"

#include "PublicDefine.h"
#include "OutOfRangeError.h"
#include "SourceLoader.h"
#include "PreProcessor.h"
#include "LexicalAnalyzer.h"
using namespace std;

#include "StringUtils.h"

using namespace XASM;

#include <map>
#include <string>
#include <iomanip>


int _tmain(int argc, _TCHAR* argv[])
{
	CSourceCodeHolder srcHolder = CSourceLoader::load_file(std::string("test.xasm"));
	srcHolder = CPreProcessor::pre_process(srcHolder);

	//cout << std::boolalpha;
	//cout << XASM::is_string_float("123") << endl;
	//cout << XASM::is_string_float("-123") << endl;
	//cout << XASM::is_string_float("1-23") << endl;
	//cout << XASM::is_string_float("12.3") << endl;
	//cout << XASM::is_string_float("123.") << endl;
	cout << endl << endl << endl;


	string s[20] = {
		"int",
		"float",
		"string",
		"quote",
		"indentify",
		"colon",
		"open_bracket",
		"close_bracket",
		"comma",
		"open_brace",
		"close_brace",
		"newline",
		"instruction",
		"set_stacksize",
		"var",
		"func",
		"param",
		"reg_retval",
		"invalid",
		"end"
	};
	CLexicalAnalyzer::Instance()->set_source_holder(srcHolder);
	ETokenType type = CLexicalAnalyzer::Instance()->get_next_token();
	while (TOKEN_TYPE_END_OF_STREAM != type)
	{
		cout << setw(10) << CLexicalAnalyzer::Instance()->get_curr_lexeme() <<setw(2) << ":" << CLexicalAnalyzer::Instance()->peed_next()<< "   :" << s[type - 1]<< endl;
		type = CLexicalAnalyzer::Instance()->get_next_token();
	}


	return 0;
}

