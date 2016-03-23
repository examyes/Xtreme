#include "TokenStream.h"

namespace XASM
{
	namespace detail
	{
		void StreamImp::push_back(shared_ptr<SToken>& token_ptr)
		{
			m_tokens.push_back(token_ptr);
		}

		StreamImp::iterator StreamImp::begin() const
		{
			return m_tokens.begin();
		}

		StreamImp::iterator StreamImp::end() const
		{
			return m_tokens.end();
		}
	}

	void CTokenStream::push_back(SToken& token)
	{
		auto token_ptr = std::make_shared<SToken>();
		*token_ptr = token;

		m_imp->push_back(token_ptr);
	}

	CTokenStream::iterator CTokenStream::begin() const
	{
		return m_imp->begin();
	}

	CTokenStream::iterator CTokenStream::end() const
	{
		return m_imp->end();
	}

	void CTokenStream::reset()
	{
		m_curr = m_imp->begin();
	}

	shared_ptr<SToken> CTokenStream::next_token()
	{
		auto token_ptr = peek_next_token();
		if (token_ptr)
		{
			m_curr++;
		}

		return token_ptr;
	}

	shared_ptr<SToken> CTokenStream::peek_next_token()
	{
		if (m_curr == m_imp->end())
		{
			return shared_ptr<SToken>(NULL);
		}

		return *m_curr;
	}
}