#pragma once

#include <memory>
#include <vector>
#include <string>
using std::shared_ptr;
using std::vector;
using std::string;

#include "PublicDefine.h"

namespace XASM
{
	namespace detail
	{
		class InstrStreamImp
		{
		public:
			using iterator = vector<shared_ptr<SInstr>>::const_iterator;
		private:
			vector<shared_ptr<SInstr>> m_instrs;

		public:
			void push_back(shared_ptr<SInstr>& instr_ptr);
			iterator begin();
			iterator end();

			size_t size();
		};
	};

	class CInstrStream
	{
	public:
		using iterator = detail::InstrStreamImp::iterator;

	private:
		shared_ptr<detail::InstrStreamImp> m_imp;

	public:
		CInstrStream(){
			m_imp = std::make_shared<detail::InstrStreamImp>();
		};

		void push_back(SInstr& instr);
		iterator begin();
		iterator end();

		size_t size();
	};
}