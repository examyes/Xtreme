#pragma once
#include <string>
#include <vector>
#include "SourceLine.h"

using std::string;
using std::vector;

//源代码文本类，代表一个源代码文本，拆分为行为单位

class SourceCodeHolder
{
public:
	SourceCodeHolder(void);
	~SourceCodeHolder(void);

	typedef SourceLine* Iterator;

public:
	void appendSourceRow(const SourceLine& sourceLine);

	Iterator begin();
	Iterator end();

	unsigned int size();
	SourceLine at(unsigned int index);

private:
	vector<SourceLine> m_sourceLineTable;
};

