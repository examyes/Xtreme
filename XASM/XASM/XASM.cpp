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

int _tmain(int argc, _TCHAR* argv[])
{
	CSourceCodeHolder srcHolder = CSourceLoader::load_file(std::string("test.xasm"));
	srcHolder = CPreProcessor::pre_process(srcHolder);

	CSourceCodeHolder::iterator itor = srcHolder.begin();
	while (itor != srcHolder.end())
	{
		cout << (*itor)->row() << ":" << (*itor)->text() << endl;
		++itor;
	}

	//cout << std::boolalpha;
	//cout << XASM::is_string_float("123") << endl;
	//cout << XASM::is_string_float("-123") << endl;
	//cout << XASM::is_string_float("1-23") << endl;
	//cout << XASM::is_string_float("12.3") << endl;
	//cout << XASM::is_string_float("123.") << endl;
	cout << endl << endl << endl;

	CLexicalAnalyzer::Instance()->set_source_holder(srcHolder);
	while (TOKEN_TYPE_END_OF_STREAM != CLexicalAnalyzer::Instance()->get_next_token())
	{
		cout << CLexicalAnalyzer::Instance()->get_curr_lexeme() << endl;
	}

	return 0;
}

