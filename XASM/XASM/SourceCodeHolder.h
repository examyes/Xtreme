#pragma once
#include <string>
#include <vector>
#include "SourceLine.h"

using std::string;
using std::vector;

//Դ�����ı��࣬����һ��Դ�����ı������Ϊ��Ϊ��λ

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

