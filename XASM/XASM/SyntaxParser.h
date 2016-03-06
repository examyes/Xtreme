#pragma once


// Óï·¨·ÖÎöÆ÷

#include "utils.h"
#include "PublicDefine.h"

namespace XASM
{
	class CSyntaxParser : public Singleton<CSyntaxParser>
	{
		friend class Singleton<CSyntaxParser>;
	private:
		SScriptHeader m_header;

	public:
		void parse();

	private:
		CSyntaxParser(void){};
		~CSyntaxParser(void){};
	};
}


