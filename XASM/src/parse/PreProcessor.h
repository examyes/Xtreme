#pragma once

#include <string>
using std::string;

#include "../data/SourceCodeHolder.h"


namespace XASM
{

class CPreProcessor
{
private:
		CPreProcessor(void) {};
		~CPreProcessor(void){};

public:
		static CSourceCodeHolder pre_process(CSourceCodeHolder& pre_holder);

private:
		static string strip_comments(const string& val_str);
		static string pre_line(const string& val_str);
};

}


