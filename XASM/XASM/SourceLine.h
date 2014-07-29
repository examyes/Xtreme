#pragma once
#include <string>

using std::string;

//Դ������

class SourceLine
{
public:
	SourceLine(string strLine,unsigned int iRowIndex);
	~SourceLine(void);

public:
	string getSourceText();
	unsigned int getRowIndex();

private:
	string m_strSourceLine;
	unsigned int m_iRowIndex;
};

