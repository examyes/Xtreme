#include "StdAfx.h"
#include "SourceLine.h"


SourceLine::SourceLine(string strLine,unsigned int iRowIndex)
	:m_strSourceLine(strLine)
	,m_iRowIndex(iRowIndex)
{

}


SourceLine::~SourceLine(void)
{

}

string SourceLine::getSourceText()
{
	return m_strSourceLine;
}

unsigned int SourceLine::getRowIndex()
{
	return m_iRowIndex;
}