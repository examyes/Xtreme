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


int main(int argc, char* argv[])
{

    CSourceCodeHolder srcHolder = CSourceLoader::load_file(std::string("test.xasm"));
    srcHolder = CPreProcessor::pre_process(srcHolder);

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
    CTokenStream token_stream = CLexicalAnalyzer::Instance()->analyze(srcHolder);

    for (auto&& i : token_stream)
    {
        cout << setw(10) << i->lexeme << setw(20) << s[i->type -1] << endl;
    }

    CSyntaxParser::Instance()->parse(token_stream);

    return 0;
}

