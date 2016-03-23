#pragma once

#include <string>
using std::string;


#include "../utils/utils.h"
#include "../data/PublicDefine.h"
#include "../data/TokenStream.h"

namespace XASM
{
	class CSyntaxParser : public Singleton<CSyntaxParser>
	{
		friend class Singleton<CSyntaxParser>;
	private:
		SScriptHeader m_header;

		int m_instr_stream_size;
		bool m_is_stacksize_found;
		bool m_is_func_active;

		int m_curr_func_local_data_size;
		int m_curr_func_index;
		string m_curr_func_name;
		int m_curr_func_param_count;

	public:
		void parse(CTokenStream& token_stream);

	private:
		CSyntaxParser(void){};
		~CSyntaxParser(void){};

		bool parse_phase_1(CTokenStream& token_stream);
		bool phase_1_identify(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool phase_1_close_brace(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool phase_1_instruction(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool phase_1_stacksize(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool phase_1_var(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool phase_1_func(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool phase_1_param(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool phase_1_default(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
	};
}


