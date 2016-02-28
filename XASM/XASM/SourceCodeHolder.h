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

	typedef CSourceLine* Iterator;

public:
	void appendSourceRow(const CSourceLine& sourceLine);

	Iterator begin();
	Iterator end();

	unsigned int size();
	CSourceLine at(unsigned int index);

private:
	vector<CSourceLine> m_sourceLineTable;
};

