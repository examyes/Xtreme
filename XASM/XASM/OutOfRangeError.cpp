#include "StdAfx.h"
#include "OutOfRangeError.h"


OutOfRangeError::OutOfRangeError(std::string strDetail)
	:m_strDetail(strDetail)
{
}


OutOfRangeError::~OutOfRangeError(void)
{
}

const char* OutOfRangeError::what() const throw()
{
	return m_strDetail.c_str();
}