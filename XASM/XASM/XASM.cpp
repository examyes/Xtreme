// XASM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "SourceCodeHolder.h"
#include "SourceLine.h"
#include "OutOfRangeError.h"
#include "SourceLoader.h"
#include "PreProcessor.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SourceCodeHolder srcHolder = SourceLoader::loadSourceFile(std::string("test.xasm"));
	srcHolder = PreProcessor::preProcess(srcHolder);

	SourceCodeHolder::Iterator itor = srcHolder.begin();
	while (itor != srcHolder.end())
	{
		cout<<itor->getRowIndex()<<":"<<itor->getSourceText()<<endl;
		++itor;
	}

	return 0;
}

