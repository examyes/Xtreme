// XASM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "SourceCodeHolder.h"
#include "SourceLine.h"
#include "OutOfRangeError.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SourceCodeHolder srcHolder;

	for (unsigned int i = 0;i < 100;++i)
	{
		srcHolder.appendSourceRow(SourceLine(string("line"),i));
	}

	SourceCodeHolder::Iterator itor = srcHolder.begin();
	while (itor != srcHolder.end())
	{
		cout<<"row "<<itor->getRowIndex()<<":"<<"  src:"<<itor->getSourceText()<<endl;
		++itor;
	}

	try
	{
		srcHolder.at(1000);
	}
	catch (OutOfRangeError& e)
	{
		cout<<e.what()<<endl;
	}
	

	return 0;
}

