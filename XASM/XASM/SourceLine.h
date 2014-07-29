#pragma once
#include <string>

using std::string;

//Ô´´úÂëÐÐ

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

