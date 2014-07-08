#pragma once
#include <exception>
#include <string>

class OutOfRangeError:public std::exception
{
public:
	OutOfRangeError(std::string strDetail);
	~OutOfRangeError(void);

public:
	const char* what() const throw();

private:
	std::string m_strDetail;
};

