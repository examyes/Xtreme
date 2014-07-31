#pragma once
#include <vector>
#include "IToken.h"

class TokenStream
{
public:
	TokenStream(void);
	~TokenStream(void);

private:
	std::vector<IToken*> m_tokenQueue;
};

