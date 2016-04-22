#include "InstrStream.h"

namespace XASM
{
	namespace detail
	{
		void InstrStreamImp::push_back(shared_ptr<SInstr>& instr_ptr)
		{
			m_instrs.push_back(instr_ptr);
		}

		InstrStreamImp::iterator InstrStreamImp::begin()
		{
			return m_instrs.begin();
		}

		InstrStreamImp::iterator InstrStreamImp::end()
		{
			return m_instrs.end();
		}

		size_t InstrStreamImp::size()
		{
			return m_instrs.size();
		}
	}

	void CInstrStream::push_back(SInstr& instr)
	{
		auto instr_ptr = std::make_shared<SInstr>();
		*instr_ptr = instr;

		m_imp->push_back(instr_ptr);
	}

	CInstrStream::iterator CInstrStream::begin()
	{
		return m_imp->begin(); 
	}

	CInstrStream::iterator CInstrStream::end()
	{
		return m_imp->end();
	}

	size_t CInstrStream::size()
	{
		return m_imp->size();
	}
}