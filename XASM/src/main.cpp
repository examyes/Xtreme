// XASM.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include "./data/SourceCodeHolder.h"

#include "./data/PublicDefine.h"
#include "./parse/SourceLoader.h"
#include "./parse/PreProcessor.h"
#include "./parse/LexicalAnalyzer.h"
#include "./parse/SyntaxParser.h"
using namespace std;

#include "./utils/StringUtils.h"

using namespace XASM;

#include <map>
#include <string>
#include <iomanip>
#include <stack>

class Solution {
public:
	/**
	* @param num: a rotated sorted array
	* @return: the minimum number in the array
	*/
	int findMin(vector<int> &num) {
		// write your code here
		int low_index = 0;
		int high_index = num.size() - 1;

		while (low_index < high_index)
		{
			int mid_index = (low_index + high_index) / 2;

			if (num[mid_index] < num[high_index])
			{
				high_index = mid_index;
			}
			else
			{
				if (mid_index == low_index)
				{
					low_index = high_index;
				}
				else
				{
					low_index = mid_index;
				}
			}
		}

		return num[low_index];
	}
};



int main(int argc, char* argv[])
{
	int i = 0;
	i = 1;
	CSourceLoader::load_file(i ? string("1") : string("2"));

	CSourceCodeHolder srcHolder = CSourceLoader::load_file(std::string("test.xasm"));
	srcHolder = CPreProcessor::pre_process(srcHolder);

	//for (auto&& i : srcHolder)
	//{
	//	cout << i->row() << "  " << i->text().size() << endl;
	//	cout << i->text() << endl;
	//}

	//string s[20] = {
	//	"int",
	//	"float",
	//	"string",
	//	"quote",
	//	"indentify",
	//	"colon",
	//	"open_bracket",
	//	"close_bracket",
	//	"comma",
	//	"open_brace",
	//	"close_brace",
	//	"newline",
	//	"instruction",
	//	"set_stacksize",
	//	"var",
	//	"func",
	//	"param",
	//	"reg_retval",
	//	"invalid",
	//	"end"
	//};
	//CTokenStream token_stream = CLexicalAnalyzer::Instance()->analyze(srcHolder);

	//for (auto&& i : token_stream)
	//{
	//	cout << setw(10) << i->lexeme << setw(20) << s[i->type - 1]<< endl;
	//}

	//CSyntaxParser::Instance()->parse(token_stream);

	Solution s;

	vector<int> n = {
		6, 1, 2, 3, 4, 5
	};
	cout << s.findMin(n) << endl;


	return 0;
}

