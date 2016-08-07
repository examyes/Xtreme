#pragma once

#include <string>
#include <liter/utils/Singleton.h>
#include <map>
#include <functional>
using std::string;
using liter::Singleton;
using std::map;
using std::function;
using std::bind;

#include "../data/PublicDefine.h"
#include "../data/TokenStream.h"
#include "../data/InstrStream.h"


namespace XASM
{

namespace detail
{

struct SyntaxParserPhase1
{
		int m_instr_stream_size;
		bool m_is_stacksize_found;
		bool m_is_func_active;

		int m_curr_func_local_data_size;
		int m_curr_func_index;
		string m_curr_func_name;
		int m_curr_func_param_count;

    bool parse(CTokenStream& token_stream);
		bool parse_identify(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool parse_close_brace(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool parse_instruction(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool parse_stacksize(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool parse_var(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool parse_func(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool parse_param(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);
		bool parse_default(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr);

private:
    using ParseFuncType = function<bool(CTokenStream&, shared_ptr<SToken>&)>;

    ParseFuncType get_parse_func(ETokenType token_type) const;
};

struct SyntaxParserPhase2
{
		int m_instr_stream_size;
		bool m_is_stacksize_found;
		bool m_is_func_active;

		int m_curr_func_local_data_size;
		int m_curr_func_index;
		string m_curr_func_name;
		int m_curr_func_param_count;

    bool parse(CTokenStream& token_stream, CInstrStream& instr_stream);
		bool parse_func(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);
		bool parse_param(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);
		bool parse_close_brace(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);
		bool parse_instruction(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);

private:
    using ParseFuncType = function<bool(CTokenStream&, shared_ptr<SToken>&, CInstrStream&)>;

    ParseFuncType get_parse_func(ETokenType token_type) const;
};

}

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

		bool parse_phase_2(CTokenStream& token_stream, CInstrStream& instr_stream);
		bool phase_2_func(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);
		bool phase_2_param(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);
		bool phase_2_close_brace(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);
		bool phase_2_instruction(CTokenStream& token_stream, shared_ptr<SToken>& token_ptr, CInstrStream& instr_stream);
};

}


