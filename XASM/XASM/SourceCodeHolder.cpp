#include "SourceCodeHolder.h"

namespace XASM
{
	namespace detail
	{
		void HolderImp::append(string& line, size_t row)
		{
			auto line_ptr = std::make_shared<CSourceCodeLine>(line, row);
			append(line_ptr);
		}

		void HolderImp::append(shared_ptr<CSourceCodeLine>& line_ptr)
		{
			m_code.push_back(line_ptr);
		}


		HolderImp::iterator HolderImp::begin()
		{
			return m_code.begin();
		}

		HolderImp::iterator HolderImp::end()
		{
			return m_code.end();
		}

		size_t HolderImp::row_count()
		{
			return m_code.size();
		}

		shared_ptr<CSourceCodeLine> HolderImp::at(size_t row)
		{
			if (row < m_code.size())
			{
				return m_code.at(row);
			}

			return shared_ptr<CSourceCodeLine>(NULL);
		}

		shared_ptr<CSourceCodeLine> HolderImp::operator[](size_t row)
		{
			return m_code[row];
		}
	}

	void CSourceCodeHolder::append(string& line, size_t row)
	{
		m_holder->append(line, row);
	}

	void CSourceCodeHolder::append(shared_ptr<CSourceCodeLine>& line_ptr)
	{
		m_holder->append(line_ptr);
	}


	CSourceCodeHolder::iterator CSourceCodeHolder::begin()
	{
		return m_holder->begin();
	}

	CSourceCodeHolder::iterator CSourceCodeHolder::end()
	{
		return m_holder->end();
	}

	size_t CSourceCodeHolder::row_count()
	{
		return m_holder->row_count();
	}

	shared_ptr<CSourceCodeLine> CSourceCodeHolder::at(size_t row)
	{
		return m_holder->at(row);
	}

	shared_ptr<CSourceCodeLine> CSourceCodeHolder::operator[](size_t row)
	{
		return (*m_holder)[row];
	}
}
