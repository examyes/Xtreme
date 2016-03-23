#pragma once


// Óï·¨·ÖÎöÆ÷

#include "../utils/utils.h"
#include "../data/PublicDefine.h"

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


