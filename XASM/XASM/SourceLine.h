#pragma once
#include <string>

using std::string;

//Ô´´úÂëÐÐ

class CSourceLine
{
public:
	CSourceLine(string strLine,unsigned int iRowIndex);
	~CSourceLine(void);

public:
	string getSourceText();
	unsigned int getRowIndex();

private:
	string m_strSourceLine;
	unsigned int m_iRowIndex;
};

