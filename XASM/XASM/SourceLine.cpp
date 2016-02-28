#include "StdAfx.h"
#include "SourceLine.h"


CSourceLine::CSourceLine(string strLine,unsigned int iRowIndex)
	:m_strSourceLine(strLine)
	,m_iRowIndex(iRowIndex)
{

}


CSourceLine::~CSourceLine(void)
{

}

string CSourceLine::getSourceText()
{
	return m_strSourceLine;
}

unsigned int CSourceLine::getRowIndex()
{
	return m_iRowIndex;
}