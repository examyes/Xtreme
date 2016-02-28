#include "SourceCodeHolder.h"
#include "OutOfRangeError.h"

SourceCodeHolder::SourceCodeHolder(void)
{
	m_sourceLineTable.clear();
}


SourceCodeHolder::~SourceCodeHolder(void)
{
	m_sourceLineTable.clear();
}

void SourceCodeHolder::appendSourceRow(const CSourceLine& sourceLine)
{
	m_sourceLineTable.push_back(sourceLine);
}

SourceCodeHolder::Iterator SourceCodeHolder::begin()
{
	if (0 != m_sourceLineTable.size())
	{
		return &m_sourceLineTable.at(0);
	}

	return NULL;
}

SourceCodeHolder::Iterator SourceCodeHolder::end()
{
	if (0 != m_sourceLineTable.size())
	{
		Iterator itor = &m_sourceLineTable.at(m_sourceLineTable.size() - 1);
		return ++itor;
	}

	return NULL;
}

unsigned int SourceCodeHolder::size()
{
	return m_sourceLineTable.size();
}

CSourceLine SourceCodeHolder::at(unsigned int index)
{
	if (index < m_sourceLineTable.size())
	{
		return m_sourceLineTable.at(index);
	}
	
	throw OutOfRangeError(string("the index out of Source Size"));
}